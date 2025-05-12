#ifndef WEBSOCKETMANAGER_HPP
#define WEBSOCKETMANAGER_HPP

#include <Arduino.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <functional>
#include <Client.h>

class WebSocketManager {
public:
    WebSocketManager(Client* client, const String& url);
    void connect();
    void loop();
    void send(const char* message);
    void send_json(const char* topic, const StaticJsonDocument<200>& doc);
    void set_event_callback(std::function<void(WStype_t, uint8_t*, size_t)> callback);

private:
    WebSocketsClient webSocket;
    Client* _client;
    String _url;
    String _host;
    String _path;
    int _port;
    bool _ssl = false;

    void parse_url();
    void on_event(WStype_t type, uint8_t* payload, size_t length);
    std::function<void(WStype_t, uint8_t*, size_t)> _event_callback;
};

#endif
