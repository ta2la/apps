# apps
Collection of end-user applications that integrate the static libraries provided by this repository.

It is necessary to upload (clone) the `base2` repository at the same directory level as `apps`.

Example directory structure (`tree`):

```
.
├── apps
└── base2
```

Furthermore: entire directories, such as `apps/PROMPT_ASSEMBLER`, build only the executable itself.  
To open the full project (the executable including all subdirectories), it is necessary to open:

apps/PROMPT_ASSEMBLER/prompt_assembler/prompt_assembler.pro

```
.
├── apps
│   └── PROMPT_ASSEMBLER
│       └── prompt_assembler
│           └── prompt_assembler.pro
└── base2
    └── ...
```
The basic functionality is to integrate files of your choice in single file. The example of the file is here.
It is assembly of this project itself.

[PROMPT ASSEMBLY](https://ta2la.github.io/apps/PROMPT_ASSEMBLER/001_example/base.h)

The prompt then can be used for prompting LLM to generate new features of the code.
           
[test EXAMPLE_001](https://ta2la.github.io/apps/PROMPT_ASSEMBLER/001_example/001_example.html)

The code generation is done by pressing **@view** button. It is hardcoded for displaying the prompt of this code itself.
If you need other, you must replace these lines in `Cmds_code_analyzer` and recompile. Just list of modules. Searches all subdirs of each module.

```
inline static QStringList dirs_ = QStringList()
        << "../../../base2/base/"
        << "../../../base2/cmd_sys"
        << "../../../base2/cmd_sys_display"
        << "../../../base2/code_analyzer"
        << "../../../apky/PROMPT_ASSEMBLER"
        << "../../../base2/utility";
```

You can limit the size of output by the following directives:

```
///@view:beg
...
///@view:end

///@view:exclude - excludes single line between beg-end
///@view:exclude - exports single line without beg-end

```

**.dot** button generates .dot graph of the same code.

![dot.png](https://ta2la.github.io/apps/PROMPT_ASSEMBLER/images/dot.png)
