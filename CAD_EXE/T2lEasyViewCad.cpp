//
// Copyright (C) 2019 Petr Talla. [petr.talla@gmail.com]
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
#include "T2lEasyViewCad.h"
#include "T2lHtmlTab.h"

#include "T2lHtmlTabsRegistry.h"

#include "TcCmdEngine.h"
#include "TcSysInfo.h"

#include "FlowLayout.h"

#include "T2lEasyViewTml.h"
#include "T2lActiveFile.h"
#include "T2lGFile.h"
#include "T2lWidgetFile.h"
#include "T2lWidgetFileCol.h"

#include <QGraphicsProxyWidget>

#include <QToolBar>
#include <QToolBar>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextStream>
#include <QDesktopServices>
#include "T2lEasyViewHtml.h"
#include "T2lEasyViewButton.h"
#include <QStackedLayout>
#include <QMessageBox>

#include "TcCmdTransl.h"

#include <iostream>

//#include <QApplication>
#include <T2lEasyViewTab.h>

using namespace T2l;
using namespace std;

#define TAB_HEIGHT 40

//=============================================================================
EasyViewCad::EasyViewCad() :
    EasyView("")
{
}

//=============================================================================
QWidget* EasyViewCad::createDebug()
{
    QVBoxLayout *layout = new QVBoxLayout;

    EXECUTE("tab_set_debug");
    HtmlTab* tab1 = HtmlTabsRegistry::instance().tabGet_("debug");
    EasyViewTml* tabWidget1 = loadTml(tab1->content(), "debug");
    tabWidget1->resize(800, 300);
    layout->addWidget(tabWidget1);

    layout->addStretch();

    QWidget* cadWidget = new QWidget();
    cadWidget->setLayout(layout);

    return cadWidget;
}

//=============================================================================
QWidget* EasyViewCad::createDir2()
{
    QVBoxLayout *layout = new QVBoxLayout;

    EXECUTE("tab_set_dir2_header")
    HtmlTab* tab1 = HtmlTabsRegistry::instance().tabGet_("dir2_header");
    EasyViewTml* tabWidget1 = loadTml(QString::fromStdString(tab1->content()), "dir2_header");
    tabWidget1->resize(800, 300);
    layout->addWidget(tabWidget1);

    QWidget *separator1 = new QWidget;
    separator1->setFixedHeight(1);
    separator1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    separator1->setStyleSheet(QString("background-color: #c0c0c0;"));
    layout->addWidget(separator1);

    EXECUTE("tab_set_dir2")
    HtmlTab* tab2 = HtmlTabsRegistry::instance().tabGet_("dir2");
    EasyViewTml* tabWidget2 = loadTml(tab2->content(), "dir2");
    tabWidget2->resize(800, 300);
    layout->addWidget(tabWidget2);

    layout->addStretch();

    QWidget* widget = new QWidget();
    widget->setLayout(layout);

    return widget;
}

//=============================================================================
QWidget* EasyViewCad::createOfiles2()
{
    QVBoxLayout *layout = new QVBoxLayout;

    EXECUTE("tab_set_ofiles2")
    HtmlTab* tab1 = HtmlTabsRegistry::instance().tabGet_("ofiles2");
    EasyViewTml* tabWidget1 = loadTml(tab1->content(), "ofiles2");
    tabWidget1->resize(800, 300);
    layout->addWidget(tabWidget1);

    QWidget *separator1 = new QWidget;
    separator1->setFixedHeight(1);
    separator1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    separator1->setStyleSheet(QString("background-color: #c0c0c0;"));
    layout->addWidget(separator1);

    EXECUTE("tab_set_ofiles2_cmds")
    HtmlTab* tab2 = HtmlTabsRegistry::instance().tabGet_("ofiles2_cmds");
    EasyViewTml* tabWidget2 = loadTml(tab2->content(), "ofiles2_cmds");
    tabWidget2->resize(800, 300);
    layout->addWidget(tabWidget2);

    layout->addStretch();

    QWidget* widget = new QWidget();
    widget->setLayout(layout);

    return widget;
}

//=============================================================================
QWidget* EasyViewCad::createCad()
{
    QVBoxLayout *layout = new QVBoxLayout;

    std::string cmd1("tab_set_"); cmd1 += "toolbar1";
    EXECUTE(cmd1.c_str())
    HtmlTab* tab1 = HtmlTabsRegistry::instance().tabGet_("toolbar1");
    EasyViewTml* tabWidget1 = loadTml(tab1->content(), "toolbar1");
    tabWidget1->resize(800, 300);
    layout->addWidget(tabWidget1);

    QWidget *separator1 = new QWidget;
    separator1->setFixedHeight(1);
    separator1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    separator1->setStyleSheet(QString("background-color: #c0c0c0;"));
    layout->addWidget(separator1);

    std::string cmd("tab_set_"); cmd += "toolbar";
    EXECUTE(cmd.c_str())
    HtmlTab* tab = HtmlTabsRegistry::instance().tabGet_("cad_toolbar");
    EasyViewTml* tabWidget = loadTml(tab->content(), "cad_toolbar");
    tabWidget->resize(800, 300);
    layout->addWidget(tabWidget);

    QWidget *separator = new QWidget;
    separator->setFixedHeight(1);
    separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    separator->setStyleSheet(QString("background-color: #c0c0c0;"));
    layout->addWidget(separator);

    tabWidget = loadTml("empty", "acmd");
    tabWidget->resize(800, 300);
    layout->addWidget(tabWidget);

    tabWidget = loadTml("empty", "icmd");
    tabWidget->resize(800, 300);
    layout->addWidget(tabWidget);

    layout->addStretch();

    QWidget* cadWidget = new QWidget();
    cadWidget->setLayout(layout);

    return cadWidget;
}

//=============================================================================
void EasyViewCad::closeEvent (QCloseEvent *event) {
    ActiveFile* af = ActiveFile::activeGet();
    if (af != nullptr && af->file()->changeRegistry() > 0) {
        QMessageBox::StandardButton button = QMessageBox::question(&EasyViewCad::instance(),
            "Continuation Question",
            "The active file has unsaved changes. CONTINUE?",
            QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::No) {
            event->ignore();;
            return;
        }
    }

    while( WidgetFile::col().count() > 0) {
        delete WidgetFile::col().get(0);
    }

    event->accept();
}

//=============================================================================
