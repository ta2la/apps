#pragma once

#include <CmdSys.h>
#include "Model_controls.h"
#include <QString>

//=============================================================================
class Cmds_ui_model_refresh {
public:
    Cmds_ui_model_refresh() = delete;
    static bool registerCmds_()
    {
        CMD_SYS.add("change_controls", change_controls, "crase", true, true);
        return true;
    }
protected:
    static int change_controls(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) {
        Q_UNUSED(data) Q_UNUSED(context)
        if (args.count() < 2) return 0;
        Model_controls::inst().setContent(args.get(1).value());
        return 0;
    }
};
