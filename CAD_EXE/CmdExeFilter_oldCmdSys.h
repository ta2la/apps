#pragma once

#include "CmdExeFilter.h"
#include "CmdSys.h"
#include <TcCmdEngine.h>
#include <TcArgCol.h>

//=============================================================================
class CmdExeFilter_oldCmdSys : public CmdExeFilter {
//=============================================================================
public:
    static CmdExeFilter_oldCmdSys& inst() {
        static CmdExeFilter_oldCmdSys i;
        return i;
    }

    bool pass(CmdArgCol& args) override {
        QString name = args.get(0).value();
        if (!name.startsWith("@")) return false;

        QString oldArgs = args.toString();
        oldArgs = oldArgs.mid(1);

        TcArgCol tcArgs(oldArgs.toStdString().c_str());
        TcCmdEngine::engine().execute(&tcArgs);
        return true;
    }

private:
    CmdExeFilter_oldCmdSys() { CMD_SYS.reg(this); }
};
