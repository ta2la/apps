# PROMPT_ASSEMBLER Project

## Overview
CLI tool for analyzing and assembling code, visualizing module dependencies, and managing code analysis operations. Qt-based with console output. Build configuration defined in `PROMPT_ASSEMBLER.pro`.

## Architecture
- GUI application, reads commands from stdin, outputs to stdout
- All commands go through `execute_threadSafe()` - queued via Qt event loop, processed sequentially
- Multiple commands can be sent in succession - they queue up and execute one by one
- No re-entrant command execution - commands from UI, scripts, and stdin all use the same queue
- Platform-specific setup (pipes, logging) described in `CLAUDE_linux.md`

## Project Structure

### Source Modules
- **./** (executable) - Main application
  - Main.cpp - Entry point
  - Model_controls.h - Command control definitions
  - Model_tabs.h - UI tab structure
  - Cmds_ui_model_refresh.h/cpp - UI model refresh command
  - UiControl.h - UI control interface

- **../../base2/** (static libraries)
  - code_analyzer/ - Code analysis engine
  - code_data/ - Code data structures
  - cmd_sys/ - Command system framework
  - cmd_sys_display/ - Command display utilities
  - object_registry/ - Object registry system
  - object_registry_test/ - Registry testing
  - file_manager/ - File management system
  - mccp_com/ - MCP communication
  - base/ - Base utilities
  - utility/ - General utilities

### Build System
- Qt project file: `PROMPT_ASSEMBLER.pro`
- Build directory: debug/ or release/ (configured via qmake)
- Standard: C++17
- Define: CUT_DEEP_DEPS
- Qt Modules: widgets, quick, quickwidgets, quickcontrols2

## Commands

When a new command is added, update this list accordingly.

```
voidcmd                                          - no-op
logcmd <message...>                              - no-op, execution IS the log entry
cmds_execute_script <file>                       - execute script (mines commands from file)
execute_script <file>                            - execute script line by line

module_add <dirPath> [--strict] [--subdirs] [--notload]  - add directory as code module
file_add <filePath> [--strict] [--lines "from to"]...    - add file to module
set_output_dir <path>                            - set output directory
dir_merge_files [--views] [--bydist]             - merge module files into output
dir_export_dot                                   - export dependency graph as .dot

set_module_used <module> <0|1>                   - mark module used/unused
analyzer_set_center <nodeName> <moduleName>      - set center node, recalculate distances
update_claude_md <moduleName>                    - generate CLAUDE.md with module info

system_open_path <path>                          - open with system default app
system_dot_to_svg <file.dot>                     - convert .dot to SVG via graphviz
file_to_clipboard <file>                         - copy file content to clipboard

ui_model_display_what <what>                     - toggle UI part visibility
change_controls <state>                          - update UI control model

oreg_test_setup                                  - initialize object registry test
oreg_test_display_pool                           - display object pool
oreg_test_create_container                       - create test container
oreg_test_container_display <index>              - display container by index
oreg_test_pool_solve                             - solve pool dependencies
oreg_list_containers                             - list all containers

create_object_test <value>...                    - create test objects
create_model_test <from> <to>                    - create test model with range
delete_object_test <id>...                       - delete test objects by ID
display_model_col_test                           - display all test models

pa_exit                                          - quit application
```

## Build Instructions
```bash
qmake PROMPT_ASSEMBLER.pro
make
```

## Key Dependencies
Build order (from PROMPT_ASSEMBLER.pro):
1. Base libraries (base, utility)
2. Infrastructure (cmd_sys, cmd_sys_display, object_registry)
3. Code analysis (code_analyzer, code_data)
4. File management (file_manager)
5. Communication (mccp_com)
6. Testing (object_registry_test)
7. Main executable

---

For general CLI architecture, code analysis guidelines, logging conventions, coding rules, and commit message format, see `../../CLAUDE/CLAUDE.md`
