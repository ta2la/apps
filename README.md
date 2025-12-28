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

Currently, it contains only the **PROMPT ASSEMBLER** application.<br>
PROMPT ASSEMBLER scans selected source directories and extracts source files, then generates a structured output that can be used directly as a prompt for an LLM.

Its primary purpose is to assemble multiple source files—selected by directory and filtering rules—into a single, coherent prompt file.
The example output shown below is generated from this project itself:

[PROMPT ASSEMBLY](https://ta2la.github.io/apps/PROMPT_ASSEMBLER/001_example/base.h)

The resulting prompt can be used to ask an LLM to analyze the codebase or propose new features and modifications.
           
[test EXAMPLE_001](https://ta2la.github.io/apps/PROMPT_ASSEMBLER/001_example/001_example.html)

Prompt generation is triggered by pressing the **@view** button.<br>
At the moment, this action is hardcoded to generate a prompt for the PROMPT ASSEMBLER project itself.<br>
To generate prompts for a different project, the directory list in Cmds_code_analyzer must be modified and the application recompiled.

```
inline static QStringList dirs_ = QStringList()
        << "../../../base2/base/"
        << "../../../base2/cmd_sys"
        << "../../../base2/cmd_sys_display"
        << "../../../base2/code_analyzer"
        << "../../../apky/PROMPT_ASSEMBLER"
        << "../../../base2/utility";
```

The tool supports output size control using inline view directives embedded in source files:

```
///@view:beg
...
///@view:end

///@view:exclude - excludes single line between beg-end
///@view:exclude - exports single line without beg-end

```

The **.dot** button generates a Graphviz .dot file representing the same codebase structure.<br>
This graph can be further converted to SVG for visualization.

![dot.png](https://ta2la.github.io/apps/PROMPT_ASSEMBLER/images/dot.png)
