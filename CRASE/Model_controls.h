#pragma once

#include "CmdModel_controls.h"

//=============================================================================
class Model_controls : public CmdModel_controls
{
    Q_OBJECT
public:
    static Model_controls& inst() { static Model_controls i; return i; }
    void setContent(const QString& content) override
    {
        content_ = content;
        beginResetModel();
        m_items.clear();

        if (content == "test") {
            m_items.append({TextType,   "TESTS OO: ",                   ""});
            m_items.append({ButtonType, "oreg_test_display_pool",       "oreg_test_display_pool"});
            m_items.append({ButtonType, "oreg_list_containers",         "oreg_list_containers"});
            m_items.append({TextType,   " --- ",                        ""});
            m_items.append({ButtonType, "oreg_test_setup",              "oreg_test_setup"});
            m_items.append({ButtonType, "oreg_test_create_container",   "oreg_test_create_container"});
            m_items.append({ButtonType, "oreg_test_container_display",  "oreg_test_container_display"});
            m_items.append({ButtonType, "oreg_test_pool_solve",         "oreg_test_pool_solve"});

            m_items.append({TextType,   "TESTS CMDS: ",             ""});
            m_items.append({ButtonType, "argcol_to_string",         "argcol_to_string"});
            m_items.append({ButtonType, "argcol_from_string",       "argcol_from_string"});
            m_items.append({ButtonType, "argcol_from_string2",      "argcol_from_string2"});
            m_items.append({ButtonType, "argcol_from_string_alone", "argcol_from_string_alone"});
        }
        if (content == "mode") {
            m_items.append({TextType,   "Active mode for selection events:", ""});
            m_items.append({ButtonType, "voidcmd",          "crase_set_mode voidcmd"});
            m_items.append({ButtonType, "cmd_mode_box",     "crase_set_mode cmd_mode_box"});

            m_items.append({ButtonType, "cmd_mode_tree",    "crase_set_mode cmd_mode_tree"});
            m_items.append({ButtonType, "cmd_mode_drawing", "crase_set_mode cmd_mode_drawing"});
            m_items.append({ButtonType, "cmd_mode_claude",  "crase_set_mode cmd_mode_claude"});
            m_items.append({ButtonType, "cmd_mode_draw",    "crase_set_mode cmd_mode_draw"});
            m_items.append({ButtonType, "cmd_mode_undraw",  "crase_set_mode cmd_mode_undraw"});
            m_items.append({ButtonType, "cmd_mode_filter",  "crase_set_mode cmd_mode_filter"});
            m_items.append({ButtonType, "cmd_mode_raw",     "crase_set_mode cmd_mode_raw"});
            m_items.append({ButtonType, "cmd_mode_delete",  "crase_set_mode cmd_mode_delete"});
        }
        endResetModel();
    }
};
