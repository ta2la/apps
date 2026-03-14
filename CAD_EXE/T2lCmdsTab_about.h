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
#pragma once

#include <TcCmdEngine.h>
#include <QString>

namespace T2l {

//=============================================================================
class  CmdsTab_about {
//=============================================================================
public:
//<CMDS>
//=============================================================================
    //<REGISTRATOR>
    static bool registerCmds_()
    {   TcCmd* cmd;
        cmd = TcCmdEngine::engine().registerCmd("tab_set_about", tab_set_about, "tab_set");
        cmd->frontControllExclude();

        return true;
    }
//<INTERNALS>
    CmdsTab_about();

    static int tab_set_about(TcCmdContext* context, TcArgCol& args);
};

} //namespace T2l
