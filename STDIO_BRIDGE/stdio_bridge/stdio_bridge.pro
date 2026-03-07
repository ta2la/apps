TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += ../../../base2/base
SUBDIRS += ../../../base2/cmd_sys
SUBDIRS += ../../STDIO_BRIDGE

../../STDIO_BRIDGE.depends = \
    ../../../base2/base \
    ../../../base2/cmd_sys
