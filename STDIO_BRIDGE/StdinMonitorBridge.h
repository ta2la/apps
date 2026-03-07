#pragma once

#include "StdinMonitor.h"

//=============================================================================
class StdinMonitorBridge : public StdinMonitor {
public:
    static StdinMonitorBridge& init() { static StdinMonitorBridge i; i.start(); return i; }
protected:
    void processLine(const QString& line) override {
        if (line.startsWith("./")) {
            StdinMonitor::processLine(line);
        }
    }
};
