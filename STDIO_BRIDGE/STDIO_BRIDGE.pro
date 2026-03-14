TARGET = STDIO_BRIDGE
TEMPLATE = app
CONFIG += c++17
CONFIG += console
QT += network websockets
QT -= gui
#######################################################################################

CONFIG(debug, debug|release): {
    SOURCEDIR = debug
} else {
    SOURCEDIR = release
}

DESTDIR = $$PWD/$$SOURCEDIR

message(DESTDIR = $$PWD/$$SOURCEDIR)

HEADERS += $$PWD/StdinMonitorBridge.h
HEADERS += $$PWD/WsClientBridge.h
HEADERS += $$PWD/Cmds_stdio_bridge.h
SOURCES += $$PWD/Main.cpp

T2LS += cmd_sys
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

    message(FC_LIB_NAME = $$FC_LIB_NAME)
}
