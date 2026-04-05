//=============================================================================
// MD_RAG_COLAB — markdown RAG collaboration viewer
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
#include "Model_tabs.h"
#include "StdoutCmdOutput.h"
//#include "WsServerLite.h"
#include "WsServerLiteGuard.h"
#include "Cmds_app_common.h"
#include "AppStyle.h"
#include "CmdExeRecCol.h"
#include "ExerecModelProxy.h"
#include "AppPaths.h"
#include "StdinMonitor.h"
#include "MdModel.h"
#include "MdItem.h"
//#include "MdParser.h"
#include "Cmds_md_rag.h"
#include "Cmds_file_manager_base.h"
#include "DirModelBase.h"
#include "BookmarkModel.h"
#include "FileItemData.h"

//! @section Qt
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

//=============================================================================
int main(int argc, char *argv[]) {
//! @section Command registration
    Cmds_test0::registerCmds_();
    Cmds_ui_model_refresh::registerCmds_();
    Cmds_cmd_sys::registerCmds_();
    Cmds_exerec::registerCmds_();
    Cmds_app_common::registerCmds();
    Cmds_md_rag::registerCmds();

//! @section Application
    QGuiApplication app(argc, argv);
    app.setApplicationName("MD RAG Colab");

    static DirModelBase mdDirModelInst;
    Cmds_file_manager_base::registerSetDir("md_set_dir", mdDirModelInst);
    Cmds_file_manager_base::registerSetBook("bookmark_set_file", BookmarkModel::inst());

//! @section View
    Q_INIT_RESOURCE(cmd_sys_display);
    Q_INIT_RESOURCE(file_manager_base);
    Q_INIT_RESOURCE(md_rag_colab);

    qRegisterMetaType<MdItem>();
    qRegisterMetaType<FileItemData>();

    AppStyle::inst().setBarColor("#4A7A5A");
    AppStyle::inst().setTextColor("#F0F0D0");
    AppStyle::inst().setIcon("\xF0\x9F\x93\x84");

    QQuickView* view = new QQuickView();
    view->rootContext()->setContextProperty("appStyle", &AppStyle::inst());
    view->rootContext()->setContextProperty("qmlInterface",       &UiControl::inst());
    view->rootContext()->setContextProperty("exerecModelProxy",   &ExerecModelProxy::inst());
    view->rootContext()->setContextProperty("cmdline",            &Model_controls::inst());
    view->rootContext()->setContextProperty("toolControls",       &Model_controlsSettings::inst());
    view->rootContext()->setContextProperty("mainTabs",           new Model_tabs());
    view->rootContext()->setContextProperty("mdModel",            &MdModel::inst());
    view->rootContext()->setContextProperty("mdDirModel",         &mdDirModelInst);
    view->rootContext()->setContextProperty("bookmarkModel",      &BookmarkModel::inst());
    view->rootContext()->setContextProperty("configDir",           AppPaths::inst().dirConfig());

//! @section Default content
    MdModel::inst().load({ MdItem(MdItem::HEADING1, { MdWord("Nothing"), MdWord("loaded.") }) });

    CmdExeRecCol::inst();
    StdoutCmdOutput::inst();
    WsServerLiteGuard::inst();

    view->setSource(QUrl("qrc:/GenericApp.qml"));
    UiControl::inst().setRootObject(view->rootObject());
    Cmds_app_common::setRootItem(view->rootObject());

    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->resize(900, 600);
    view->show();

    QObject::connect(&app, &QGuiApplication::aboutToQuit, []() {
        StdinMonitor::init().requestInterruption();
        fclose(stdin);
        StdinMonitor::init().wait(1000);
        QCoreApplication::quit();
    });

    QString script = QDir::cleanPath(AppPaths::inst().dirConfig() + "/config.t2l");
    if (QFileInfo::exists(script))
        CMD_SYS.execute_threadSafe("execute_script " + script);
    CMD_SYS.execute_threadSafe("voidcmd");

//! @section Run
    return app.exec();
}
