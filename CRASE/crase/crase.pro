TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += ../../../base2/base2
SUBDIRS += ../../../base2/object_registry
SUBDIRS += ../../../base2/object_registry_test
SUBDIRS += ../../../base2/cmd_sys
SUBDIRS += ../../../base2/cmd_sys_display
SUBDIRS += ../../../base2/utility
SUBDIRS += ../../../base2/app_common
SUBDIRS += ../../../base2/crase_viewer
SUBDIRS += ../../../base2/mccp_com
SUBDIRS += ../../CRASE

../../CRASE.depends = \
    ../../../base2/base2 \
    ../../../base2/object_registry \
    ../../../base2/object_registry_test \
    ../../../base2/cmd_sys \
    ../../../base2/cmd_sys_display \
    ../../../base2/utility \
    ../../../base2/app_common \
    ../../../base2/crase_viewer \
    ../../../base2/mccp_com
