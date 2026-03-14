//
// Copyright (C) 2013 Kinalisoft. [kinalisoft.eu]
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
// author: petr.talla@kinalisoft.eu
//   date: 2012-06-17
//purpose:
//=============================================================================
//self
#include "T2lCmdsTab_cad_toolbar.h"

#include "T2lHtmlTab.h"

//#include "T2lCadExeSettings.h"
#include "T2lEasyViewTml.h"
#include "T2lEasyApp.h"

//cad
#include "T2lActiveFile.h"
#include "T2lStoredFileNames.h"
#include "T2lCadSettings.h"
#include "T2lGFile.h"
#include "T2lGFileCol.h"

// infrastructure
#include "T2lHtmlTabsRegistry.h"
//#include "TcHtmlViewTabbed.h"
//#include "TcHtmlDir.h"
#include "TcArgCol.h"
#include "TcArgVal.h"
#include "TcCmdLog.h"
#include "T2lCmds_htmlView.h"

// hg
#include "T2lWidgetInteract.h"
#include "T2lFilter.h"
#include "T2lCmdQueue.h"
#include "T2lCmd.h"

#include <QString>
#include <QDir>
#include <QCoreApplication>
#include <QTextStream>

using namespace T2l;

//=============================================================================
CmdsTab_cad_toolbar::CmdsTab_cad_toolbar()
{
}

//=============================================================================
QString CmdsTab_cad_toolbar::button(const QString& symbol, const QString& cmd)
{
    QString acmd = cmd;
    if (acmd.isEmpty()) acmd = symbol;

    QString result = "TC;CB;cmd: ";
    result += acmd + ";icon: ";
    result += StoredFileNames::getExeUpDir() + "/resource/icons/";
    result += symbol;
    result += ".png;;";

    return result;
}

//=============================================================================
int CmdsTab_cad_toolbar::tab_set_toolbar(TcCmdContext* /*context*/, TcArgCol& /*args*/)
{
    QString result;

    result += "TC;CT;text: select:;;";
    result += button( "edit_select",      "cmd_object_select" );
    result += "TC;CT;text: <space>;;";

    result += "TC;CT;text: create:;;";
    result += button("create_line",           "cad_draw_line_pro" );
    result += button("route_segment_draw",    "cmd_draw_route_segment" );
    result += button("route_segment_connect", "cmd_connect_route_segment" );

    result += button("create_area",   "cmd_cad_draw_area" );
    result += button("create_symbol", "cmd_object_enter");
    result += button("create_image",  "cmd_cad_draw_image");
    result += button("create_text",   "cmd_cad_draw_text");
    result += button("create_dimm",   "cmd_cad_draw_dimm");

    result += "TC;CT;text: <space>;;";
    result += "TC;CT;text: copy:;;";
    result += button("copy",              "cmd_object_copy");
    result += button("copy_parallel",     "cmd_object_mocopy_parallel");

    result += "TC;CT;text: <space>;;";
    result += "TC;CT;text: edit:;;";
    result += button( "edit_move",        "cmd_object_move"          );
    result += button( "edit_move_points", "cmd_object_movepoints"    );
    result += button( "edit_trim",        "cmd_object_trim"          );
    result += button( "edit_trimto",      "cmd_object_trimtoother"   );
    result += button( "edit_split",       "cmd_object_split"   );
    result += button( "transform_image",  "cmd_cad_transform_image"   );
    result += button( "image_properties", "cmd_cad_image_properties"   );
    result += button( "edit_symbology",   "cmd_object_set_symbology" );
    result += button( "change_text",      "change_text" );
    result += button( "delete",           "cmd_object_delete"        );

    result += "TC;CT;text: <space>;;";
    result += "TC;CT;text: info:;;";
    result += button("info_measure", "cmd_cad_measure");

    result += "TC;CT;text: <space>;;";
    result += "TC;CT;text: view:;;";
    result += button("view_pan",        "cmd_view_pan");
    result += button("view_zoomrect",   "cmd_view_zoomrect");
    result += button("view_fit", "cmd_cad_view_fit");
    result += button("view_info", "cmd_cad_view_info");

    result += "TC;CT;text: <space>;;";
    result += "TC;CT;text: completion:;;";
    result += button("cmd_cad_active_file_move", "cmd_cad_active_file_move");
    result += button("cad_draw_papper", "cad_draw_papper");
    result += button("cad_print", "cad_print");

/*
    result += "TC;CT;text: <space>;;";
    result += "TC;CT;text: settings:;;";
    result += button("cad_settings", "cad_settings");
*/

    //===================================================
    result = result.replace("TC", "type: control");
    result = result.replace("CT", "control: text");
    result = result.replace("CB", "control: button");
    result = result.replace(";", "\n");

    HtmlTab* tab = HtmlTabsRegistry::instance().tabGet_("cad_toolbar");
    tab->contentSet(result.toStdString().c_str());

    return 0;
}

//=============================================================================
