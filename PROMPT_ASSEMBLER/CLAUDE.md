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
  - mcp_com/ - MCP communication
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
voidcmd
logcmd <message...>
cmds_execute_script <file>
execute_script <file>

module_add <dirPath> [--strict] [--subdirs] [--notload]
file_add <filePath> [--strict] [--lines "from to"]...
set_output_dir <path>
dir_merge_files [--views] [--bydist]
dir_export_dot

set_module_used <module> <0|1>
analyzer_set_center <nodeName> <moduleName>
update_claude_md <moduleName>

system_open_path <path>
system_dot_to_svg <file.dot>
file_to_clipboard <file>

ui_model_display_what <what>
change_controls <state>

oreg_test_setup
oreg_test_display_pool
oreg_test_create_container
oreg_test_container_display <index>
oreg_test_pool_solve
oreg_list_containers

create_object_test <value>...
create_model_test <from> <to>
delete_object_test <id>...
display_model_col_test
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
5. Communication (mcp_com)
6. Testing (object_registry_test)
7. Main executable

---

For general CLI architecture, code analysis guidelines, logging conventions, and coding rules, see `../../CLAUDE.md`
