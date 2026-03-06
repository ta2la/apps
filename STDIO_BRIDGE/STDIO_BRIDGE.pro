TARGET = STDIO_BRIDGE
TEMPLATE = app
CONFIG += c++17
CONFIG += console
QT += network
QT -= gui
#######################################################################################

CONFIG(debug, debug|release): {
    SOURCEDIR = debug
} else {
    SOURCEDIR = release
}

DESTDIR = $$PWD/$$SOURCEDIR

message(DESTDIR = $$PWD/$$SOURCEDIR)
