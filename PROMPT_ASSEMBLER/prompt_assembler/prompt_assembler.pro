TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += ../../../base2/base
SUBDIRS += ../../../base2/cmd_sys
SUBDIRS += ../../../base2/cmd_sys_display
SUBDIRS += ../../../base2/utility
SUBDIRS += ../../../base2/code_analyzer
SUBDIRS += ../../PROMPT_ASSEMBLER

../../PROMPT_ASSEMBLER.depends = \
    ../../../base2/base \
    ../../../base2/cmd_sys \
    ../../../base2/cmd_sys_display \
    ../../../base2/utility \
    ../../../base2/code_analyzer
