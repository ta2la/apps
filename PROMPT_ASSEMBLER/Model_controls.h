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
 * @class Model_controls
 * @brief Model for test ui buttons and text entries.
 */
#pragma once

#include "Model_controlsEx.h"
#include "Model_filteroutCmds.h"

#include <QAbstractListModel>
#include <QList>
#include <QString>

///@view:beg

class Model_controls : public Model_controlsEx
//=============================================================================
{
    Q_OBJECT
public:
    static Model_controls& inst() { static Model_controls i; return i; }
    void setContent(const QString& content)
    {
        content_ = content;

        beginResetModel();

        m_items.clear();

        if (content == "start_stop") {
            m_items.append({TextType,   "EXPORT: ", ""});
            m_items.append({ButtonType, "@view", "dir_merge_files --views"});

            m_items.append({TextType,   "NET: ", ""});
            m_items.append({ButtonType, ".dot",  "dir_export_dot"});
            m_items.append({ButtonType, "Load",  "dir_load_net"});
            m_items.append({ButtonType, "Debug", "dir_load_net_debug"});

            m_items.append({TextType,   "TEST: ", ""});
            m_items.append({ButtonType, "Voidcmd",      "voidcmd"});
            m_items.append({ButtonType, "Includes",     "dir_list_includes_test"});
            m_items.append({ButtonType, "List Dirs",    "dir_list"});
            m_items.append({ButtonType, "Export Merge", "dir_merge_files"});
            m_items.append({ButtonType, "pass_dir",     "pass_dir"});
            m_items.append({ButtonType, "select_dir",   "select_dir"});
            m_items.append({ButtonType, "Bad Command",  "bad_command"});
        }
        if (content == "disabled") {
            m_items.append({TextType, "Disabled commands:", ""});
            Model_filteroutCmds& model = Model_filteroutCmds::inst();
            for (int i = 0; i < model.cmds_.count(); i++) {
                QString cmdi = model.cmds_[i];
                m_items.append({ButtonType, cmdi, QString("exerec_remove_filterout_command ") + cmdi});
            }
        }
        if (content == "test") {
            m_items.append({TextType,   "TESTS CMDS: ",             ""});
            m_items.append({ButtonType, "argcol_to_string",         "argcol_to_string"});
            m_items.append({ButtonType, "argcol_from_string",       "argcol_from_string"});
            m_items.append({ButtonType, "argcol_from_string2",      "argcol_from_string2"});
            m_items.append({ButtonType, "argcol_from_string_alone", "argcol_from_string_alone"});
        }

        endResetModel();
    }
};

///@view:end
