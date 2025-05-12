#include "include/can.hpp"
#include "include/gsm_7600.hpp"
#include "include/data_processor.hpp"
#include "include/websocket_manager.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();

DataProcessor dataProcessor;
CAN canController;
GSM7600 gsm7600;

WebSocketManager websocket(gsm7600.get_client(), "ws://localhost:8080/ws");

void check_connections(void* pvParameters){
    while(true){
        Serial.println("Checking connections...");
        if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE){
            if (!gsm7600.check_connection()) {
                Serial.println("GSM connection failed, attempting to reconnect...");
                delay(5000);
                gsm7600.connect();
            }
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("G24EVO::GSMController - Attempting LTE Connection...");
    
    dataProcessor.set_mutex(xMutex);
    gsm7600.set_mutex(xMutex);

    gsm7600.begin();
    if (!gsm7600.is_connected()) {
        Serial.println("Failed to connect to the network");
        return;
    }

    canController.start();

    // Conexión WebSocket
    websocket.connect();
    Serial.println("G24EVO::WebSocket - Connected to WebSocket Server!");

    dataProcessor.set_websocket_manager(&websocket);
    canController.set_data_proccessor(&dataProcessor);

    xTaskCreatePinnedToCore(
        check_connections,
        "CheckConnections",
        4096,
        NULL,
        2,
        NULL,
        1
    );  
}

void loop(){ 
    websocket.loop(); 
    canController.listen();
    vTaskDelay(pdMS_TO_TICKS(10));
}
