#include "StdoutCmdOutput.h"
#include "Cmds_cmd_sys.h"
#include "StdinMonitorBridge.h"

#include <QCoreApplication>

//=============================================================================
int main(int argc, char *argv[]) {
    Cmds_cmd_sys::registerCmds_();

    QCoreApplication app(argc, argv);
    StdoutCmdOutput::inst();
    StdinMonitorBridge::init();

    return app.exec();
}
