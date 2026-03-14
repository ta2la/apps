TARGET = t2lcad
TEMPLATE = app
#######################################################################################
CONFIG += c++11
DESTDIR = $$PWD/debug

#QMAKE_LFLAGS += -sGL_ENABLE_GET_PROC_ADDRESS

SOURCES  += $$PWD/*.cpp
HEADERS  += $$PWD/*.h

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/

DEPLIBS += infrastructure:applib:include
DEPLIBS += evo:cad
DEPLIBS += evo:cad_draw
DEPLIBS += cad_professional:cad_professional_lib
DEPLIBS += evo:cad_infrastructure
DEPLIBS += evo:cad_attrs
DEPLIBS += cad:cad_settings
#DEPLIBS += evo:route_model

DEPLIBS += evo:storage
DEPLIBS += evo:storage_base:include

DEPLIBS += hg:hg_interact
DEPLIBS += hg:hg_display
DEPLIBS += hg:hg_style
DEPLIBS += hg:hg_papper
DEPLIBS += hg:hg_utility:include
DEPLIBS += hg:hg_text

DEPLIBS += infrastructure:html_view

DEPLIBS += infrastructure:licence:include
DEPLIBS += infrastructure:object_registry
DEPLIBS += infrastructure:command_registry_ex
DEPLIBS += infrastructure:command_registry

DEPLIBS += base:geogebra
DEPLIBS += base:base:include

CONFIG(debug, debug|release): {
    SOURCEDIR = debug
} else {
    SOURCEDIR = release
}

message("======================================")

for (LIBID, DEPLIBS) {
    DIRNAME = $$section(LIBID, :, 0, 0)
    LIBNAME = $$section(LIBID, :, 1, 1)
    SUBNAME = $$section(LIBID, :, 2, 2)

    INCLUDEPATHI = $$PWD/../../$${DIRNAME}/$${LIBNAME}

    !equals(SUBNAME, "") {
        INCLUDEPATHI = $${INCLUDEPATHI}/$${SUBNAME}
    }

    LIBPATH = "$$PWD/../../$${DIRNAME}/$${LIBNAME}/$${SOURCEDIR}/lib$${LIBNAME}.a"

    INCLUDEPATH += $${INCLUDEPATHI}
    LIBS += $${LIBPATH}
    PRE_TARGETDEPS += $${LIBPATH}

    message("INCUDEPATHI::$${INCLUDEPATHI}")
    message("LIBPATH::$${LIBPATH}")
}

QT += widgets
QT += quick quickwidgets

DISTFILES += $$PWD/resource/icons/*.png
RC_ICONS = $$PWD/app.ico
