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
#include "Cmds_utility_system.h"
#include "Model_tabs.h"
#include "StdoutCmdOutput.h"
#include "WsServerLite.h"
#include "WsServerLiteGuard.h"
#include "Cmds_app_common.h"
#include "AppStyle.h"
#include "CmdExeRecCol.h"
#include "ExerecModelProxy.h"
#include "AppPaths.h"
#include "StdinMonitor.h"
#include "MdModel.h"
#include "MdItem.h"

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
    Cmds_utility_system::registerCmds();
    Cmds_app_common::registerCmds();

//! @section Application
    QGuiApplication app(argc, argv);
    app.setApplicationName("MD RAG Colab");

//! @section View
    Q_INIT_RESOURCE(cmd_sys_display);
    Q_INIT_RESOURCE(utility);
    Q_INIT_RESOURCE(md_rag_colab);

    qRegisterMetaType<MdItem>();

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

//! @section Demo content
    QList<MdItem> demo;
    demo.append(MdItem(MdItem::HEADING1, "Orientace ve vykresu"));
    demo.append(MdItem(MdItem::PARAGRAPH, "Zakladem"));
    demo.append(MdItem(MdItem::PARAGRAPH, "je"));
    demo.append(MdItem(MdItem::PARAGRAPH, "orientace"));
    demo.append(MdItem(MdItem::PARAGRAPH, "ve"));
    demo.append(MdItem(MdItem::PARAGRAPH, "vykresu"));
    demo.append(MdItem(MdItem::PARAGRAPH, "a"));
    demo.append(MdItem(MdItem::PARAGRAPH, "jeho"));
    demo.append(MdItem(MdItem::PARAGRAPH, "prepis"));
    demo.append(MdItem(MdItem::PARAGRAPH, "na"));
    demo.append(MdItem(MdItem::PARAGRAPH, "text."));
    demo.append(MdItem(MdItem::PARAGRAPH, "To"));
    demo.append(MdItem(MdItem::PARAGRAPH, "je"));
    demo.append(MdItem(MdItem::PARAGRAPH, "trojsky", MdItem::BOLD));
    demo.append(MdItem(MdItem::PARAGRAPH, "kun", MdItem::BOLD));
    demo.append(MdItem(MdItem::PARAGRAPH, "—"));
    demo.append(MdItem(MdItem::PARAGRAPH, "ovladani"));
    demo.append(MdItem(MdItem::PARAGRAPH, "aplikace"));
    demo.append(MdItem(MdItem::PARAGRAPH, "skrze"));
    demo.append(MdItem(MdItem::PARAGRAPH, "AI."));

    demo.append(MdItem(MdItem::HEADING2, "Databaze projektu"));
    demo.append(MdItem(MdItem::PARAGRAPH, "Projekt"));
    demo.append(MdItem(MdItem::PARAGRAPH, "zahrnuje"));
    demo.append(MdItem(MdItem::PARAGRAPH, "databazovou"));
    demo.append(MdItem(MdItem::PARAGRAPH, "informaci"));
    demo.append(MdItem(MdItem::PARAGRAPH, "o"));
    demo.append(MdItem(MdItem::PARAGRAPH, "projektu."));
    demo.append(MdItem(MdItem::PARAGRAPH, "DB"));
    demo.append(MdItem(MdItem::PARAGRAPH, "info"));
    demo.append(MdItem(MdItem::PARAGRAPH, "je"));
    demo.append(MdItem(MdItem::PARAGRAPH, "pozemky,", MdItem::BOLD));
    demo.append(MdItem(MdItem::PARAGRAPH, "adresy,", MdItem::BOLD));
    demo.append(MdItem(MdItem::PARAGRAPH, "kontakty,", MdItem::BOLD));
    demo.append(MdItem(MdItem::PARAGRAPH, "terminy,"));
    demo.append(MdItem(MdItem::PARAGRAPH, "cisla"));
    demo.append(MdItem(MdItem::PARAGRAPH, "parcel."));

    demo.append(MdItem(MdItem::HEADING2, "Komunikace se zakaznikem"));
    demo.append(MdItem(MdItem::PARAGRAPH, "AI"));
    demo.append(MdItem(MdItem::PARAGRAPH, "muze"));
    demo.append(MdItem(MdItem::PARAGRAPH, "sledovat"));
    demo.append(MdItem(MdItem::PARAGRAPH, "historii"));
    demo.append(MdItem(MdItem::PARAGRAPH, "komunikace,"));
    demo.append(MdItem(MdItem::PARAGRAPH, "generovat"));
    demo.append(MdItem(MdItem::PARAGRAPH, "navrhy"));
    demo.append(MdItem(MdItem::PARAGRAPH, "emailu,"));
    demo.append(MdItem(MdItem::PARAGRAPH, "vest"));
    demo.append(MdItem(MdItem::PARAGRAPH, "denik", MdItem::BOLD));
    demo.append(MdItem(MdItem::PARAGRAPH, "komunikace."));
    MdModel::inst().load(demo);

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
