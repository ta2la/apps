TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += ../../../base2/base2
SUBDIRS += ../../../base2/object_registry
SUBDIRS += ../../../base2/cmd_sys
SUBDIRS += ../../../base2/cmd_sys_display
SUBDIRS += ../../../base2/utility
SUBDIRS += ../../../base2/app_common
SUBDIRS += ../../../base2/mccp_com
SUBDIRS += ../../MD_RAG_COLAB

../../MD_RAG_COLAB.depends = \
    ../../../base2/base2 \
    ../../../base2/object_registry \
    ../../../base2/cmd_sys \
    ../../../base2/cmd_sys_display \
    ../../../base2/utility \
    ../../../base2/app_common \
    ../../../base2/mccp_com
