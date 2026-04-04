TARGET = MD_RAG_COLAB
TEMPLATE = app
CONFIG += c++17
CONFIG += console
#######################################################################################

CONFIG(debug, debug|release): {
    SOURCEDIR = debug
} else {
    SOURCEDIR = release
}

DESTDIR = $$PWD/$$SOURCEDIR

INCLUDEPATH += $$PWD

HEADERS += $$PWD/Model_tabs.h
HEADERS += $$PWD/Model_controls.h
HEADERS += $$PWD/Cmds_ui_model_refresh.h
HEADERS += $$PWD/UiControl.h
HEADERS += $$PWD/Model_toolControls.h
HEADERS += $$PWD/Cmds_md_rag.h

SOURCES += $$PWD/Main.cpp

RESOURCES += $$PWD/Resources/md_rag_colab.qrc

T2LS += file_manager_md
T2LS += file_manager_base
T2LS += md_rag
T2LS += app_common
T2LS += cmd_sys_display
T2LS += cmd_sys
T2LS += object_registry
T2LS += mccp_com
T2LS += base2

defined(BUILDROOT, var): ROOT_LIB_DIR = $$BUILDROOT
else:                    ROOT_LIB_DIR = $$PWD/../../BUILD

for (T2LSLIB, T2LS) {
win32 {
    FC_LIB_NAME = $$ROOT_LIB_DIR/$$T2LSLIB/$$SOURCEDIR/$${T2LSLIB}.lib
} else {
       FC_LIB_NAME = $$ROOT_LIB_DIR/$$T2LSLIB/$$SOURCEDIR/lib$${T2LSLIB}.a
}

    INCLUDEPATH    += $$PWD/../../base2/$$T2LSLIB
    LIBS           += $$FC_LIB_NAME
    PRE_TARGETDEPS += $$FC_LIB_NAME
}

#######################################################################################

QT += widgets
QT += quick
QT += quickwidgets
QT += quickcontrols2
QT += websockets
