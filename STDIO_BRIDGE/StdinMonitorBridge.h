#pragma once

#include "StdinMonitor.h"
#include "WsClientBridge.h"

//=============================================================================
class StdinMonitorBridge : public StdinMonitor {
public:
    static StdinMonitorBridge& init() { static StdinMonitorBridge i; i.start(); return i; }
protected:
    void processLine(const QString& line) override {
        if (line.startsWith("./")) {
            StdinMonitor::processLine(line);
        } else {
            ws_->send(line);
        }
    }
public:
    void setWsClient(WsClientBridge* ws) { ws_ = ws; }
private:
    WsClientBridge* ws_ = nullptr;
};
