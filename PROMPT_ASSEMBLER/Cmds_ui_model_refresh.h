//
// Copyright (C) 2025 Petr Talla. [petr.talla@gmail.com]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//		      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=============================================================================
/**
 * @class Cmds_test
 * @brief Basic test of CmdSys.
 */
#pragma once

//#include <TcCmdEngine.h>
#include <CmdSys.h>

#include <QString>
#include <QTimer>

///@view:beg

//=============================================================================
class  Cmds_ui_model_refresh {
//=============================================================================
public:
//! @section Construction
    Cmds_ui_model_refresh() = delete;
    static bool registerCmds_()
    {
        CMD_SYS.add("ui_model_display_what",            ui_model_display_what,            false, false );
        CMD_SYS.add("change_controls",       change_controls,       true,  true );

        return true;
    }
protected:
//! @section Commands
    static int ui_model_display_what              ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int change_controls              ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );

    static QSet<QString> parts_;
    static QString ui_model_html_cmdtext_text_tab_(const QString& tabname, const QString& prefix = "&nbsp;&nbsp;&nbsp;");
};

///@view:end
