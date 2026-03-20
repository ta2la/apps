//
// Copyright (C) 2015 Petr Talla. [petr.talla@gmail.com]
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

// self
#include "T2lCmdsTab_about.h"
#include "T2lHtmlTabsRegistry.h"

#include "T2lHtmlTab.h"

// infrastructure
#include "TcArgCol.h"

using namespace std;
using namespace T2l;

//=============================================================================
int CmdsTab_about::tab_set_about(TcCmdContext* /*context*/, TcArgCol& /*args*/)
{
    QString result;

    result.append(
        "<div class='dir_part1'>"
        );
    result.append("<p><big><b>Cad2l 0.9.1</big></b></p>");
    result.append("<p>Copyright 2015-2024 Petr Talla, petr.talla@gmail.com. All right reserved.</p>");
    result.append("<p>The program is provided AS IS with NO WARRANY OF ANY KIND, "
                  "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY "
                  "AND FITNESS FOR A PARTICULAR PURPOSE.</p>");

    result.append("</hr>");
    result.append("<p>The <b>documentation</b> files can be found <a href='tcview:://#cmd_open_file_external https://t2ls.com/t2lcad/help.html'>there</a>.</p>");
    result.append("<p>The application website is <a href='tcview:://#cmd_open_file_external https://t2ls.com/t2lcad'>t2ls.com/t2lcad</a>.</p>");

    /*result.append("<br/><p>hardware id number: ");
    QByteArray huuid = QSysInfo::machineUniqueId();
    result.append(huuid.data());
    result.append("</p>");
    result.append("<p>licence number generated: <b>");
    result.append(LicenceUtility::generateLicence(huuid));
    result.append("</b></p>");
    result.append("<p>licence number stored: <b>");
    result.append(LicenceUtility::keyFileRead());
    result.append("</b></p>");*/

    HtmlTab* tab = HtmlTabsRegistry::instance().tabGet_("about");
    tab->contentSetEx(result.toStdString());

    return 0;
}

//=============================================================================
