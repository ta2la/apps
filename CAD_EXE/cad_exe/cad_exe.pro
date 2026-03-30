TEMPLATE = subdirs
CONFIG += ordered

message("======================================")

#for (LIBID, DEPLIBS) {
#    DIRNAME = $$section(LIBID, :, 0, 0)
#    LIBNAME = $$section(LIBID, :, 1, 1)

#    LIBPATH = "../../../$${DIRNAME}/$${LIBNAME}"

#    SUBDIRS += $${LIBPATH}

#    message("SUBDIRS::$${LIBPATH}")
#}

#message("======================================")
#message("======================================")

SUBDIRS += ../../../base/base
SUBDIRS += ../../../base/geogebra

SUBDIRS += ../../../infrastructure/command_registry
SUBDIRS += ../../../infrastructure/command_registry_ex
SUBDIRS += ../../../infrastructure/object_registry

SUBDIRS += ../../../infrastructure/html_view

SUBDIRS += ../../../hg/hg_text
SUBDIRS += ../../../hg/hg_utility
SUBDIRS += ../../../hg/hg_papper
SUBDIRS += ../../../hg/hg_style
SUBDIRS += ../../../hg/hg_display
SUBDIRS += ../../../hg/hg_interact

SUBDIRS += ../../../evo/storage_base
SUBDIRS += ../../../evo/storage

#SUBDIRS += ../../../evo/route_model

SUBDIRS += ../../../evo/cad_attrs
SUBDIRS += ../../../cad/cad_settings
SUBDIRS += ../../../evo/cad_infrastructure
SUBDIRS += ../../../cad_professional/cad_professional_lib
SUBDIRS += ../../../evo/cad_draw
SUBDIRS += ../../../evo/cad
SUBDIRS += ../../../evo/cad_cmds_manipulate
SUBDIRS += ../../../infrastructure/applib

SUBDIRS += ../../../base2/base2
SUBDIRS += ../../../base2/cmd_sys
SUBDIRS += ../../../base2/mccp_com

SUBDIRS += ../../CAD_EXE
