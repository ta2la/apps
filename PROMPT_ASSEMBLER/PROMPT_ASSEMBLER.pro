TARGET = PROMPT_ASSEMBLER
TEMPLATE = app
CONFIG += c++17
#######################################################################################
QMAKE_CXXFLAGS += -Werror=return-type

DEFINES += CUT_DEEP_DEPS

CONFIG(debug, debug|release): {
    SOURCEDIR = debug
} else {
    SOURCEDIR = release
}

DESTDIR = $$PWD/$$SOURCEDIR

message(DESTDIR = $$PWD/$$SOURCEDIR)

# DIRECT FILES
#######################################################################################

INCLUDEPATH += $$PWD

RESOURCES += $$PWD/resources/Monitor.qrc

HEADERS += $$PWD/Cmds_ui_model_refresh.h
HEADERS += $$PWD/UiControl.h
HEADERS += $$PWD/Model_controls.h
HEADERS += $$PWD/Model_tabs.h

SOURCES += $$PWD/Main.cpp
SOURCES += $$PWD/Cmds_ui_model_refresh.cpp

T2LS += code_analyzer
T2LS += cmd_sys_display
T2LS += cmd_sys
T2LS += utility
T2LS += base

defined(BUILDROOT, var): ROOT_LIB_DIR = $$BUILDROOT
else:                    ROOT_LIB_DIR = $$PWD/../../BUILD

for (T2LSLIB, T2LS) {
    FC_LIB_NAME = $$ROOT_LIB_DIR/$$T2LSLIB/$$SOURCEDIR/lib$${T2LSLIB}.a

    INCLUDEPATH    += $$PWD/../../base2/$$T2LSLIB
    LIBS           += $$FC_LIB_NAME
    PRE_TARGETDEPS += $$FC_LIB_NAME

    message(FC_LIB_NAME = $$FC_LIB_NAME)
}

#######################################################################################

QT += widgets
QT += websockets
QT += positioning
QT += sql

QT += quick
QT += quickwidgets
QT += quickcontrols2

#######################################################################################

QMAKE_POST_ACTION = $$system(echo -e "\a")
QMAKE_POST_ACTION += $$system(sleep 0.2)
QMAKE_POST_ACTION += $$system(echo -e "\a")


