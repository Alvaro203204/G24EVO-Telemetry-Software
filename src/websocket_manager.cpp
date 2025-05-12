#include "../include/websocket_manager.hpp"

WebSocketManager::WebSocketManager(Client* client, const String& url)
    : _client(client), _url(url) {
    parse_url();
}

void WebSocketManager::parse_url() {
    if (_url.startsWith("wss://")) {
        _ssl = true;
        _url.remove(0, 6);
        _port = 443;
    } else if (_url.startsWith("ws://")) {
        _url.remove(0, 5);
        _port = 80;
    }

    int slashIndex = _url.indexOf('/');
    _host = _url.substring(0, slashIndex);
    _path = _url.substring(slashIndex);
    
    int colonIndex = _host.indexOf(':');
    if (colonIndex != -1) {
        _port = _host.substring(colonIndex + 1).toInt();
        _host = _host.substring(0, colonIndex);
    }
}

void WebSocketManager::connect() {
    webSocket.begin(_host.c_str(), _port, _path.c_str());
    webSocket.setAuthorization("", "");
    webSocket.setReconnectInterval(5000);

    webSocket.onEvent([this](WStype_t type, uint8_t* payload, size_t length) {
        this->on_event(type, payload, length);
    });
}

void WebSocketManager::on_event(WStype_t type, uint8_t* payload, size_t length) {
    if (_event_callback) {
        _event_callback(type, payload, length);
    } else {
        if (type == WStype_CONNECTED) {
            Serial.println("WebSocket connected");
        } else if (type == WStype_DISCONNECTED) {
            Serial.println("WebSocket disconnected");
        } else if (type == WStype_TEXT) {
            Serial.printf("WebSocket message: %s\n", payload);
        }
    }
}

void WebSocketManager::set_event_callback(std::function<void(WStype_t, uint8_t*, size_t)> callback) {
    _event_callback = callback;
}

void WebSocketManager::loop() {
    webSocket.loop();
}

void WebSocketManager::send(const char* message) {
    webSocket.sendTXT(message);
}

void WebSocketManager::send_json(const char* topic, const StaticJsonDocument<200>& doc) {
    StaticJsonDocument<256> fullMessage;
    fullMessage["topic"] = topic;
    fullMessage["payload"] = doc;

    char buffer[256];
    serializeJson(fullMessage, buffer);
    send(buffer);
}
