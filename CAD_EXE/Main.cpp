//
// Copyright (C) 2013 Petr Talla. [petr.talla@gmail.com]
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
//cmds
#include "TcCmds_cmdEx.h"
#include "TcCmds_cmdEngine.h"
#include "T2lCmds_htmlView.h"
#include "T2lCmdsTab_cad.h"
#include "T2lCmds_cad.h"
//#include "T2lCmds_signal.h"
#include "T2lLicenceUtility.h"

//cad
#include "T2lTentativeImplementationCad.h"

//infrastructure
#include <TcObjectRegistry.h>
#include "T2lHtmlTabsRegistry.h"
#include <TcCmdTransl.h>
#include <TcLock.h>
#include <TcCmdExecuteLate.h>
#include <TcCmdEngine.h>
#include <T2lScript.h>
#include "T2lEasyViewCad.h"
#include "CmdExeFilter_oldCmdSys.h"
#include "T2lCadObject_refLine.h"
#include "T2lCadObject_refBoxDevelop.h"
//#include "T2lCadObject_routeSegment.h"


#include "T2lStoredFactory.h"
#include "T2lCadLine.h"
#include "T2lCadObject_area.h"
#include "T2lCadObject_image.h"
#include "T2lCadObject_symbol.h"
#include "T2lCadObject_text.h"
#include "T2lCadObject_dimm.h"
#include "T2lCadObject_papper.h"
#include "T2lCmds_cad.h"
#include "T2lCmds_cad_exe.h"
#include "T2lCmdsTab_about.h"
#include "T2lCmdsTab_cad_toolbar.h"

#include "T2lCmdsTab_dir.h"
#include "T2lCmdsTab_about.h"
#include "T2lCmds_cadPro.h"
#include "T2lCadObject_linePro.h"
#include "T2lCmds_cadSettings.h"

// hg
#include "T2lCmdQueue.h"

// qt
#include <QApplication>
#include <QScreen>
#include <QProcess>

// std
#include <iostream>

using namespace std;
using namespace T2l;

//=============================================================================
int main(int argc, char *argv[]) {
    // output
    cout << "##################################" << endl;
    cout << "cad exe started" << endl;

    // command registration
    CmdExeFilter_oldCmdSys::inst();
    TcObjectRegistry::registry();
    TcCmds_cmdEx::registerCmds_();
    TcCmds_cmdEngine();
    Cmds_htmlView::registerCmds_();
    CmdsTab_dir::registerCmds_();
    CmdsTab_about::registerCmds_();
    CmdsTab_cad::registerCmds_();
    CmdsTab_cad_toolbar::registerCmds_();
    Cmds_cad::registerCmds_();
    Cmds_cad_exe::registerCmds_();
    Cmds_cadPro::registerCmds_();
    Cmds_cadSettings();

    Point2FCol points(Point2F(0, 0)); points.add(Point2F(0, 0));

    // objects registration
    StoredFactory::instance().add(new CadObject_linePro ( Point2Col<double>(), nullptr ));
    StoredFactory::instance().add(new CadLine           ( Point2Col<double>(), nullptr ));
    StoredFactory::instance().add(new CadObject_symbol  ( Point2F(), nullptr ));
    StoredFactory::instance().add(new CadObject_image   ( "ble.png", Point2<double>(0, 0), nullptr ));
    StoredFactory::instance().add(new CadObject_text    ( "null", Point2<double>(-100000, 0), nullptr ));
    StoredFactory::instance().add(new CadObject_dimm    ( Point2F(0, 0), -1, nullptr ));
    StoredFactory::instance().add(new CadObject_refLine ( points, nullptr));
    StoredFactory::instance().add(new CadObject_area    ( points, Color(Color::BLACK), Color(Color::BLACK), nullptr, false, false));
    StoredFactory::instance().add(new CadObject_papper  ( Point2F(10e9, 10e9), 50, 1, 200, 200, nullptr));
    //StoredFactory::instance().add(new CadObject_routeSegment(Point2Col<double>(), nullptr) );

    // queue instantiation
    Queue::instance()->set(&CmdQueue::queue());
    CmdQueue::queue().setTentaiveImplementation(new TentativeImplementationCad());

    // custom scripts
    Script::run("configs", "main");
    Script::run("configs", "macros");

    // application initialization
    QApplication a(argc, argv);
    a.setApplicationName("T2CAD");

    CmdsTab_dir::subdirSet("cad");

    TcCmdExecuteLate exeLate;

    Q_INIT_RESOURCE(cad_icons);

    /*if (LicenceUtility::licenceIsValid()==false) {
        QProcess::execute("C:/HOME/KADLUB/cvz/apky/cad_exe/t2lcad_licence/t2lcad_licence.exe");
    }*/

    EasyViewCad& VIEW = EasyViewCad::instance();

    VIEW.createTab("dir2",     VIEW.createDir2(),    "Directory2");
    VIEW.createTab("ofiles2",  VIEW.createOfiles2(), "Opened Files");
    VIEW.createTab("cad",      VIEW.createCad(),     "Toolbox");
    VIEW.createTab("exed",     nullptr,              "Executed");
    VIEW.createTab("about",    nullptr,              "Help/About");
#ifdef QT_DEBUG
    VIEW.createTab("debug",    VIEW.createDebug(),   "Debug");
#endif

    XCALL("set_main_tab dir");

    EasyViewCad::instance().resize(900,500);
    EasyViewCad::instance().show();

    return a.exec();
}

//=============================================================================

