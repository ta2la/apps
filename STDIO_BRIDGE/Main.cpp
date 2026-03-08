#include "StdoutCmdOutput.h"
#include "Cmds_cmd_sys.h"
#include "Cmds_stdio_bridge.h"
#include "StdinMonitorBridge.h"

#include <QCoreApplication>

//=============================================================================
int main(int argc, char *argv[]) {
    Cmds_cmd_sys::registerCmds_();
    Cmds_stdio_bridge::registerCmds();

    QCoreApplication app(argc, argv);
    StdoutCmdOutput::inst().setEnabled(true);
    StdinMonitorBridge::init().setWsClient(&WsClientBridge::inst());

    return app.exec();
}
