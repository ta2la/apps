## Source Modules
- ./ (executable)
- ../../base2/code_analyzer (static lib)
- ../../base2/code_data (static lib)
- ../../base2/cmd_sys (static lib)
- ../../base2/object_registry (static lib)
- ../../base2/base (static lib)
- ../../base2/cmd_sys_display (static lib)
- ../../base2/utility (static lib)
- ../../base2/object_registry_test (static lib)
- ../../base2/file_manager (static lib)
- ../../base2/mcp_com (static lib)

## Command system (CLI)

- Commands available via stdin, output to stdout.
- Each command logs itself; additional info is appended to the command line in output.
- First argument is positional (unnamed); named args use `--name value`.
- `--name` can repeat.
- Arg value formats: plain, `"quoted string"`, or `{"json"}`.
- Example: `file_add path/to/file.cpp --lines 10 20`

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

## Logging (logcmd)

- `logcmd` is a no-op command — its execution IS the log entry.
- Usage: `logcmd <message>`
- Example: `logcmd prompt_registry_test start`

## If asked analyze
- Output code MUST include context: 10 lines before and 10 lines after each change.
- Mark each change with: //## beg
  and //## end
- For each change, print a header in comment: // path/to/file.ext LINES: N..M

## Rules 
 - Prefer conservative modifications consistent with the existing code.
 - Do NOT add unnecessary validations; validations only upon explicit request.
 - Modify it so that the code is framed using //--- and write a new one - also in analyze:
//--- původní řádky
 nove řádky
