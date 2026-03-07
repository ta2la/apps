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
 * @file Main.cpp
 * @brief Implementation of function main(...)
 */

//! @section Problemspace
#include "ExerecModelProxy.h"

//! @section Infrastructure
#include "UiControl.h"

//! @section Commands
#include "Cmds_cmd_sys.h"
#include "Cmds_test0.h"
#include "Cmds_ui_model_refresh.h"
#include "Cmds_exerec.h"
#include "Cmds_test0.h"
#include "Model_controls.h"
#include "Interactive_uiControl.h"
#include "Interactive_uiControl.h"
#include "Cmds_code_analyzer.h"
//#include "Cmds_code_analyzer_test.h"
#include "Cmds_utility_system.h"
#include "CmdExeRecCol.h"
#include "ExerecModelProxy.h"
#include "InteractiveOutputModel.h"
#include "AnalyzerModuleCol.h"
#include "Model_tabs.h"
#include "Cmds_oreg_test.h"
#include "Cmds_object_registry_test.h"
#include "TestModelCol_Model.h"
#include "TestModelItem.h"
#include "Cmds_code_data.h"
#include "PromptCompModel.h"
#include "PromptCompCol.h"
#include "CmdChannelStd.h"
#include "MonitorSocketCmd.h"

//! @Section QT
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QListView>

using namespace std;

///@view:beg
/// test commit - remove latter

//=============================================================================
int main(int argc, char *argv[]) {
//! @section Command registration
    Cmds_test0::registerCmds_();
    Cmds_ui_model_refresh::registerCmds_();
    Cmds_cmd_sys::registerCmds_();
    Cmds_exerec::registerCmds_();
    Cmds_test0::registerCmds_();
    Cmds_code_analyzer::registerCmds_();
    Cmds_utility_system::registerCmds();
    Cmds_oreg_test::registerCmds();
    Cmds_object_registry_test::registerCmds();
    Cmds_code_data::registerCmds_();

//! @section Application
    QGuiApplication app(argc, argv);
    app.setApplicationName("PROMPT Assembler");
    QIcon appIcon("qrc:/images/icon.svg");
    app.setWindowIcon(appIcon);

//! @section View
    Q_INIT_RESOURCE(cmd_sys_display);
    Q_INIT_RESOURCE(code_analyzer);
    Q_INIT_RESOURCE(utility);
    Q_INIT_RESOURCE(object_registry_test);

    qRegisterMetaType<AnalyzerModuleData>();
    qRegisterMetaType<TestModelItem>();

    QQuickView* view = new QQuickView();

    view->rootContext()->setContextProperty("qmlInterface",       &UiControl::inst());
    view->rootContext()->setContextProperty("exerecModelProxy",   &ExerecModelProxy::inst());
    view->rootContext()->setContextProperty("cmdline",            &Model_controls::inst());
    view->rootContext()->setContextProperty("interactiveIface",   &Interactive_uiControl::inst());
    view->rootContext()->setContextProperty("interactiveOutput",  &InteractiveOutputModel::inst());
    view->rootContext()->setContextProperty("mainTabs",           new Model_tabs());
    view->rootContext()->setContextProperty("analyzerModules",    &AnalyzerModuleCol::inst() );
    view->rootContext()->setContextProperty("promptModel",        &PromptCompModel::inst());
    view->rootContext()->setContextProperty("testModel",          &TestModelCol_Model::inst());

    CmdExeRecCol::inst();
    CmdChannelStd::inst();
    PromptCompCol::inst();
    MonitorSocketCmd::inst();

    view->setSource(QUrl("qrc:/GenericApp.qml"));
    UiControl::inst().setRootObject(view->rootObject());

    view->setPosition(450,650);
    view->resize(700,1500);
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->show();

    CMD_SYS.execute_threadSafe("voidcmd");
    CMD_SYS.execute_threadSafe("change_controls start_stop");

//! @section Run
    return app.exec();
}

///@view:end

//=============================================================================
