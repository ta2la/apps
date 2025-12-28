<style>
/* obecný markdown kontejner */
.md {
  font-family: system-ui, sans-serif;
  line-height: 1.5;

  background: #f5f5f5;   /* světlé pozadí stránky */
  color: #111;           /* tmavý text */
  padding: 16px;         /* aby text neležel na okraji */
}

/* PROMPT */
.md .prompt {
  background: #ecdbe4ff;          /* červeno-fialový nádech */
  border-left: 8px solid #c44269ff;
  padding: 12px;
  margin: 12px 0;
}

/* ANSWER */
.md .answer {
  background: #d4d0d0ff;          /* šedý */
  border-left: 8px solid #69668dff;
  padding: 12px;
  margin: 12px 0;
}

/* code blocky */
.md pre {
  background: #00000000;
  padding: 10px;
  overflow-x: auto;

  border: none;
  outline: none;
  box-shadow: none;
  border-left: 1px solid #AAAAAA;
}

.md code {
  font-family: monospace;
  font-size: 11px;
  line-height: 1.2;
  color: #111;
}
</style>

**First documented attempts to use prompt generator to generate its own code. The generated code commit is here: [COMMIT](https://github.com/ta2la/base2/commit/7f55a0d9895a5552e91b21ef4386afc926c3a867)**

<div class="md">

<div class="prompt">

  **PPROMPT:**<br>
  **[PROMPT ASSEMBLY](./base.h)**<br>
  \+<br>
  So now we will create a class into which we will put Cmds_code_analyzer::dirs_<br>
  but as a class, so the item that will contain a QString will be named AnalyzerModule<br>
  the class with the collection will then be AnalyzerModuleCol<br>
  leave dirs_ for now, I will swap it later

  **REPAIR PROMPT**:

  Please use the same style as in the other code_analyzer classes.

  **REPAIR PROMPT:**:
  
  In AnalyzerModuleCol, provide a get by index; do not use iterators.
</div>

<div class="answer">

  **ANSWER:**

    class AnalyzerModule
    //=============================================================================
    {
    public:
    /// @section Construction
    explicit AnalyzerModule(const QString& dirPath)
        : dirPath_(dirPath) {}

    /// @section Accessors
    const QString& dirPath() const { return dirPath_; }

    QString moduleName() const { return QFileInfo(dirPath_).dirName(); }

    bool exists() const { return QDir(dirPath_).exists(); }

    //=============================================================================
    protected:
    /// @section Data
    QString dirPath_;
    };

    class AnalyzerModuleCol
    //=============================================================================
    {
    public:
    /// @section Construction
    AnalyzerModuleCol() = default;

    /// @section Methods
    void add(const QString& dir)
    {
        modules_.append(AnalyzerModule(dir));
    }

    int count() const
    {
        return modules_.count();
    }

    bool isEmpty() const
    {
        return modules_.isEmpty();
    }

    const AnalyzerModule& get(int index) const
    {
        return modules_.at(index);
    }

    //=============================================================================
    protected:
    /// @section Data
    QList<AnalyzerModule> modules_;
    };
</div>    

<div class="prompt">

  **PROMPT:**

    class AnalyzerModule
    //=============================================================================
    {
    public:
    /// @section Construction
    explicit AnalyzerModule(const QString& dirPath)
        : dirPath_(dirPath)
    {
    }

    /// @section Accessors
    const QString& dirPath() const { return dirPath_; }

    //=============================================================================
    protected:
    /// @section Data
    QString dirPath_;
    };

    class AnalyzerModuleCol
    //=============================================================================
    {
    public:
    /// @section Construction
    AnalyzerModuleCol() = default;

    /// @section Methods
    void add(const QString& dir)
    {
        modules_.append(AnalyzerModule(dir));
    }

    int count() const
    {
        return modules_.count();
    }

    const AnalyzerModule& get(int index) const
    {
        return modules_.at(index);
    }

    //=============================================================================
    protected:
    /// @section Data
    QList<AnalyzerModule> modules_;
    };
  perfecto<br>
  Now add a method that returns a QStringList, so that we can perform a cheap substitution where, for example, …
</div>

<div class="answer">

  **ANSWER:**

    QStringList toStringList() const
    {
        QStringList result;

        for (int i = 0; i < modules_.count(); ++i) {
            result.append(modules_.at(i).dirPath());
        }

        return result;
    }
</div>    

</div>