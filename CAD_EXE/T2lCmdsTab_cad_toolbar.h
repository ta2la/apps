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
#pragma once

#include "./TcCmdEngine.h"
class TcArgCol;

#include <QString>

namespace T2l {

class CmdsTab_cad_toolbar
{
//=============================================================================
public:
//<CMDS>
    static int tab_set_toolbar ( TcCmdContext* context, TcArgCol& args );
//=============================================================================
//<REGISTRATOR>
    static bool registerCmds_()
    {   TcCmd* cmd;

        cmd = TcCmdEngine::engine().registerCmd( "tab_set_cad_toolbar", tab_set_toolbar,  "tab_toolbar");
        cmd->frontControllExclude();

        return true;
    }
//=============================================================================
protected:
//<INTERNALS>
    CmdsTab_cad_toolbar();
    static QString button(const QString& symbol, const QString& cmd = "");
};

} //namespace T2l
