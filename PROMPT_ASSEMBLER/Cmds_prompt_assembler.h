#pragma once

#include "CmdSys.h"
#include "StdinMonitor.h"
#include <QCoreApplication>

//=============================================================================
class Cmds_prompt_assembler {
//=============================================================================
public:
    Cmds_prompt_assembler() = delete;
    static bool registerCmds_()
    {
        CMD_SYS.add("pa_exit",
        []CMD_ARGS_U -> int {
            StdinMonitor::init().requestInterruption();
            fclose(stdin);
            StdinMonitor::init().wait(1000);
            QCoreApplication::quit();
            return 0;
        });

        return true;
    }
};
