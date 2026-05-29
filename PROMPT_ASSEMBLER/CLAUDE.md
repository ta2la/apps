# PROMPT_ASSEMBLER Project

**IMPORTANT:** At session start, always read `../../CLAUDE/CLAUDE.md` — it contains shared conventions, bridge connection procedure, coding rules, and architecture notes. When user says "claude" or "do claude", it refers to that file (`../../CLAUDE/CLAUDE.md`).

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

To get the current command list, use `cmds_list` via bridge. Output: `--CATEGORY <category> : cmd1 cmd2 ...`

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
