#pragma once

#include "CmdSys.h"
#include "MdParser.h"
#include "MdModel.h"

//=============================================================================
class Cmds_md_rag {
public:
    static void registerCmds() {
        CMD_SYS.add("md_load",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            if (args.count() < 2) return -1;
            QString path = args.get(1).value();
            QList<MdItem> items = MdParser::fromFile(path);
            MdModel::inst().load(items);
            return 0;
        });
    }
};
