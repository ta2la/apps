TARGET = CRASE
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

SOURCES += $$PWD/Main.cpp

RESOURCES += $$PWD/Resources/crase_app.qrc

DEPLIBS += app_components:crase_viewer
DEPLIBS += app_components:crase_drawing
DEPLIBS += app_components:crase_db
DEPLIBS += base2:app_common
DEPLIBS += base2:cmd_sys_display
DEPLIBS += base2:cmd_sys
DEPLIBS += base2:utility
DEPLIBS += base2:object_registry
DEPLIBS += base2:object_registry_test
DEPLIBS += base2:mccp_com
DEPLIBS += base2:base2

defined(BUILDROOT, var): ROOT_LIB_DIR = $$BUILDROOT
else:                    ROOT_LIB_DIR = $$PWD/../../BUILD

for (LIBID, DEPLIBS) {
    DIRNAME = $$section(LIBID, :, 0, 0)
    LIBNAME = $$section(LIBID, :, 1, 1)

win32 {
    FC_LIB_NAME = $$ROOT_LIB_DIR/$$LIBNAME/$$SOURCEDIR/$${LIBNAME}.lib
} else {
       FC_LIB_NAME = $$ROOT_LIB_DIR/$$LIBNAME/$$SOURCEDIR/lib$${LIBNAME}.a
}

    INCLUDEPATH    += $$PWD/../../$$DIRNAME/$$LIBNAME
    LIBS           += $$FC_LIB_NAME
    PRE_TARGETDEPS += $$FC_LIB_NAME
}

#######################################################################################

QT += widgets
QT += quick
QT += quickwidgets
QT += quickcontrols2
QT += websockets
QT += sql
