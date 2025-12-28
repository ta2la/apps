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

[test EXAMPLE_001](https://ta2la.github.io/docs//PROMPT_ASSEMBLER/001_example/001_example.html)
