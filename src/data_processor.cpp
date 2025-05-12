#include "../include/data_processor.hpp"

char* DataProcessor::process(std::vector<float> data){
    static char buffer[256];
    StaticJsonDocument<200> doc;

    doc["timestamp"] = millis();
    doc["device_id"] = "G24";
    doc["RPM"] = data[0];

    serializeJson(doc, buffer);

    return buffer;
}

void DataProcessor::set_websocket_manager(WebSocketManager *ws_manager) {
    _ws_manager = ws_manager;
}

void DataProcessor::send_data(const char* topic, StaticJsonDocument<200>& doc) {
    char buffer[200];
    serializeJson(doc, buffer);
    _ws_manager->send(topic, buffer);
}

void DataProcessor::test(int i, int j){
    StaticJsonDocument<200> doc;
    doc["rpm"] = i;
    doc["tps"] = j;
    send_data(test_topic, doc);
}

void DataProcessor::send_gps_data(float lat, float lng, float speed){
    StaticJsonDocument<200> doc;
    doc["lat"] = lat;
    doc["lng"] = lng;
    doc["speed"] = speed;
    send_data(gps_topic, doc);
}

void DataProcessor::send_satellites_data(int satellites){
    StaticJsonDocument<200> doc;
    doc["satellites"] = satellites;
    send_data(satellites_topic, doc);
}

void DataProcessor::send_frame_0(int rpmh, int rpml, int tpsh, int tpsl, int brkh, int brkl, int gear){
    StaticJsonDocument<200> doc;
    doc["rpm"] = (rpmh * 256) + rpml;
    doc["tps"] = (tpsh * 256) + tpsl;
    doc["ect"] = (brkh * 256) + brkl;
    doc["gear"] = gear;
    send_data(can_frame_0_topic, doc);
}

void DataProcessor::send_frame_1(int brkh, int brkl, int lrws, int rrws, int maph, int mapl, int ect){
    StaticJsonDocument<200> doc;
    doc["brk"] = (brkh * 256) + brkl;
    doc["ect"] = ect;
    send_data(can_frame_1_topic, doc);
}

void DataProcessor::send_frame_2(int lambh, int lambl, int lamth, int lamtl, int bvolth, int bvoltl, int iat){
    StaticJsonDocument<200> doc;
    doc["lamb"] = (lambh * 256) + lambl;
    doc["lamt"] = (lamth * 256) + lamtl;
    doc["bvol"] = (bvolth * 256) + bvoltl;
    doc["iat"] = iat;
    send_data(can_frame_2_topic, doc);
}