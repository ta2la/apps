#pragma once

#include "CmdExeFilter.h"
#include "CmdSys.h"
#include <TcCmdTransl.h>

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

        QString oldCmd = args.toString();
        oldCmd = oldCmd.mid(1);
        TcCmdTransl::xcall(oldCmd.toStdString().c_str());
        return true;
    }

private:
    CmdExeFilter_oldCmdSys() { CMD_SYS.reg(this); }
};
