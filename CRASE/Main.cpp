//=============================================================================
// Crase — database object viewer
//=============================================================================

//! @section Infrastructure
#include "UiControl.h"

//! @section Commands
#include "Cmds_cmd_sys.h"
#include "Cmds_test0.h"
#include "Cmds_ui_model_refresh.h"
#include "Cmds_exerec.h"
#include "Model_controls.h"
#include "Model_toolControls.h"
#include "Cmds_utility_system.h"
#include "Cmds_oreg_test.h"
#include "Cmds_object_registry_test.h"
#include "TestModelCol_Model.h"
#include "TestModelItem.h"
#include "Model_tabs.h"
#include "StdoutCmdOutput.h"
#include "WsServerLite.h"
#include "WsServerLiteGuard.h"
//#include "PreviewModel.h"
#include "Cmds_app_common.h"
#include "AppStyle.h"
#include "CraseObjectsBySqlModel.h"
#include "CraseObject.h"
#include "CraseTreeModel.h"
#include "CraseTreeItem.h"
#include "CraseDrawingModel.h"
#include "CraseDrawingItem.h"
#include "CraseTypesModel.h"
#include "CraseTypeItem.h"
#include "CraseAttrTypesModel.h"
#include "CraseAttrTypeItem.h"
#include "CraseRelTypesModel.h"
#include "CraseRelTypeItem.h"
#include "Cmds_crase_viewer.h"
#include "Cmds_crase_drawing.h"
#include "CraseMode.h"
#include "CraseSelection.h"
#include "ClaudeOutput.h"
#include "ObjectRaw.h"
#include "DbProfiles.h"
#include "CraseEmFs.h"
#include "AttrBinaryImageProvider.h"
#include "CmdExeRecCol.h"
#include "ExerecModelProxy.h"
#include "AppPaths.h"
#include "StdinMonitor.h"

//! @section Qt
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QCoreApplication>
#include <QDir>
#include <QTimer>
#include <QFileInfo>

//=============================================================================
int main(int argc, char *argv[]) {
//! @section Command registration
    Cmds_test0::registerCmds_();
    Cmds_ui_model_refresh::registerCmds_();
    Cmds_cmd_sys::registerCmds_();
    Cmds_exerec::registerCmds_();
    Cmds_utility_system::registerCmds();
    Cmds_oreg_test::registerCmds();
    Cmds_object_registry_test::registerCmds();
    Cmds_app_common::registerCmds();
    Cmds_crase_viewer::registerCmds();
    Cmds_crase_drawing::registerCmds();

//! @section Application
    QGuiApplication app(argc, argv);
    app.setApplicationName("Crase");
    StdinMonitor::init();  // create singleton early; lazy creation in aboutToQuit crashes

//! @section View
    Q_INIT_RESOURCE(cmd_sys_display);
    Q_INIT_RESOURCE(utility);
    Q_INIT_RESOURCE(object_registry_test);
    Q_INIT_RESOURCE(crase_viewer);
    Q_INIT_RESOURCE(crase_drawing);
    Q_INIT_RESOURCE(crase_app);

    qRegisterMetaType<TestModelItem>();
    qRegisterMetaType<CraseObject>();
    qRegisterMetaType<CraseTreeItem>();
    qRegisterMetaType<CraseDrawingItem>();
    qRegisterMetaType<CraseTypeItem>();
    qRegisterMetaType<CraseAttrTypeItem>();
    qRegisterMetaType<CraseRelTypeItem>();

    AppStyle::inst().setBarColor("#5680A0");
    AppStyle::inst().setTextColor("#F0DC98");
    AppStyle::inst().setIcon("\xF0\x9F\x8C\x80");

    QQuickView* view = new QQuickView();
    view->engine()->addImageProvider("binary", new AttrBinaryImageProvider());
    view->rootContext()->setContextProperty("appStyle", &AppStyle::inst());

    view->rootContext()->setContextProperty("qmlInterface",       &UiControl::inst());
    view->rootContext()->setContextProperty("exerecModelProxy",   &ExerecModelProxy::inst());
    view->rootContext()->setContextProperty("cmdline",            &Model_controls::inst());
    view->rootContext()->setContextProperty("toolControls",       &Model_controlsSettings::inst());
    view->rootContext()->setContextProperty("mainTabs",           new Model_tabs());
    view->rootContext()->setContextProperty("testModel",          &TestModelCol_Model::inst());
    //view->rootContext()->setContextProperty("previewModel",       &PreviewModel::inst());

    static CraseObjectsBySqlModel craseObjectsModel;
    static CraseTreeModel craseTreeModel;
    static CraseDrawingModel craseDrawingModel;
    Cmds_crase_viewer::setModel(&craseObjectsModel);
    Cmds_crase_viewer::setTreeModel(&craseTreeModel);
    Cmds_crase_viewer::setDrawingModel(&craseDrawingModel);
    Cmds_crase_drawing::setDrawingModel(&craseDrawingModel);
    view->rootContext()->setContextProperty("craseObjectsModel",   &craseObjectsModel);
    view->rootContext()->setContextProperty("craseTreeModel",      &craseTreeModel);
    view->rootContext()->setContextProperty("craseDrawingModel",   &craseDrawingModel);

    static CraseTypesModel craseTypesModel;
    static CraseAttrTypesModel craseAttrTypesModel;
    static CraseRelTypesModel craseRelTypesModel;
    Cmds_crase_viewer::setTypesModel(&craseTypesModel);
    Cmds_crase_viewer::setAttrTypesModel(&craseAttrTypesModel);
    Cmds_crase_viewer::setRelTypesModel(&craseRelTypesModel);
    view->rootContext()->setContextProperty("craseTypesModel",      &craseTypesModel);
    view->rootContext()->setContextProperty("craseAttrTypesModel",  &craseAttrTypesModel);
    view->rootContext()->setContextProperty("craseRelTypesModel",   &craseRelTypesModel);
    view->rootContext()->setContextProperty("craseMode",            &CraseMode::inst());
    view->rootContext()->setContextProperty("craseSelection",       &CraseSelection::inst());
    view->rootContext()->setContextProperty("claudeOutput",         &ClaudeOutput::inst());
    view->rootContext()->setContextProperty("objectRaw",            &ObjectRaw::inst());
    view->rootContext()->setContextProperty("dbProfiles",           &DbProfiles::inst());

    CmdExeRecCol::inst();
    StdoutCmdOutput::inst();
    WsServerLiteGuard::inst();

    view->setSource(QUrl("qrc:/GenericApp.qml"));
    UiControl::inst().setRootObject(view->rootObject());
    Cmds_app_common::setRootItem(view->rootObject());

    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->resize(1200, 800);
    view->show();

    QObject::connect(&app, &QGuiApplication::aboutToQuit, []() {
        auto& mon = StdinMonitor::init();
        mon.requestInterruption();
        fclose(stdin);
        mon.wait(1000);
    });

#ifdef Q_OS_WASM
    // WASM: showDirectoryPicker requires a user gesture, so the bootstrap
    // (pickDir → download tanks.db → db_connect) must be triggered from a
    // QML button — see cmd "wasm_pick_dir" registered in Cmds_crase_viewer.
#else
    QString script = QDir::cleanPath(AppPaths::inst().dirConfig() + "/config.t2l");
    if (QFileInfo::exists(script))
        CMD_SYS.execute_threadSafe("execute_script " + script);
#endif
    CMD_SYS.execute_threadSafe("voidcmd");

    // startup: wait for layout, then arrange 2x2 grid + load drawing + fit
    QTimer::singleShot(200, []() {
        CMD_SYS.execute_threadSafe("set_mdi_window_pct 0  5  5 44 39");    // Interactive
        CMD_SYS.execute_threadSafe("set_mdi_window_pct 2 51  5 44 39");    // Objects
        CMD_SYS.execute_threadSafe("set_mdi_window_pct 3  5 46 44 49");    // Tree
        CMD_SYS.execute_threadSafe("set_mdi_window_pct 4 51 46 44 49");    // Drawing
        CMD_SYS.execute_threadSafe("crase_draw 179");
        CMD_SYS.execute_threadSafe("crase_set_mode cmd_mode_tree");
    });
    QTimer::singleShot(700, []() { CMD_SYS.execute_threadSafe("zoom_all"); });

//! @section Run
    return app.exec();
}
