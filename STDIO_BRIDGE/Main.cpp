#include "CmdChannelStd.h"
#include "Cmds_cmd_sys.h"
#include "MonitorSocketCmd.h"

#include <QCoreApplication>

//=============================================================================
int main(int argc, char *argv[]) {
    Cmds_cmd_sys::registerCmds_();

    QCoreApplication app(argc, argv);
    CmdChannelStd::inst();
    MonitorSocketCmd::inst();

    return app.exec();
}
