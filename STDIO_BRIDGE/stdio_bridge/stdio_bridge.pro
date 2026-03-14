TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += ../../../base2/base2
SUBDIRS += ../../../base2/cmd_sys
SUBDIRS += ../../STDIO_BRIDGE

../../STDIO_BRIDGE.depends = \
    ../../../base2/base2 \
    ../../../base2/cmd_sys
