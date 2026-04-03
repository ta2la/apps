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
    using W = MdWord;
    using I = MdItem;
    QList<MdItem> demo;

    demo.append(I(I::HEADING1, { W("Cesnecka") }));
    demo.append(I(I::PARAGRAPH, { W("Klasicka"), W("ceska"), W("polevka"), W("na"),
        W("zahrati."), W("Idealni"), W("po"), W("silvestru.") }));

    demo.append(I(I::HEADING2, { W("Ingredience") }));
    demo.append(I(I::PARAGRAPH, { W("8"), W("strouzku", W::BOLD), W("cesneku,"),
        W("4"), W("brambory,"), W("litr"), W("vody,"), W("kmyn,"), W("sul.") }));
    demo.append(I(I::PARAGRAPH, { W("Volitelne:"), W("vejce,"), W("opeceny"), W("chleb,"),
        W("syr", W::BOLD), W("na"), W("strouhanem.") }));

    demo.append(I(I::HEADING2, { W("Postup") }));
    demo.append(I(I::PARAGRAPH, { W("Brambory"), W("oloupat,"), W("nakrajet"), W("na"),
        W("kosticky"), W("a"), W("dat"), W("varit"), W("do"), W("osolene"), W("vody.") }));
    demo.append(I(I::PARAGRAPH, { W("Cesnek"), W("prolisovat"), W("nebo"), W("najemno"),
        W("nakrajet."), W("Pridat"), W("k"), W("bramboram"), W("az"), W("zmeknou.") }));
    demo.append(I(I::PARAGRAPH, { W("Ochutit"), W("kmynem", W::BOLD), W("a"), W("soli."),
        W("Podat"), W("s"), W("chlebem"), W("a"), W("vejcem.") }));

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
