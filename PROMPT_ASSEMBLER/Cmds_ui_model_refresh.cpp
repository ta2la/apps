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
 * @file Cmds_test.cpp
 * @brief Implementation of Cmds_test class.
 */
//=============================================================================
#include "Cmds_ui_model_refresh.h"

#include "UiControl.h"

//#include "HtmlFormat.h"
#include "Model_controls.h"

#include <QVariant>
#include <QTextStream>
#include <QDebug>

///@view:beg

QSet<QString> Cmds_ui_model_refresh::parts_ = {"tests"};

//=============================================================================
int Cmds_ui_model_refresh::change_controls(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    if (args.count() < 2) return 0;

    CmdArg argText = args.get(1);

    Model_controls::inst().setContent(argText.value());

    return 0;
}

//=============================================================================
int Cmds_ui_model_refresh::ui_model_display_what ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context )
{
    if (args.count() < 2)  return 0;

    QString value = args.get(1).value();

    if (parts_.contains(value)) {
        parts_.remove(value);
    }
    else {
        parts_.insert(value);
    }

    return 0;
}

//=============================================================================
QString Cmds_ui_model_refresh::ui_model_html_cmdtext_text_tab_(const QString& tabname, const QString& prefix )
{
    QString boldBeg, boldEnd;

    if (parts_.contains(tabname)) {
        boldBeg = "<b>";
        boldEnd = "</b>";
    }

    QString result;
    QTextStream stream(&result);
    stream << prefix << boldBeg << "<a href='ui_model_display_what "
           << tabname << "'>[" << tabname << "]</a>" << boldEnd;

    return result;
}

///@view:end

//=============================================================================
