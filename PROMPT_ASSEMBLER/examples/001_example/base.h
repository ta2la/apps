
// ---- FILE: /home/rce/KADLUB/cvz/base2/base/CmdArg.h ----


//=============================================================================
class  CmdArg {
//=============================================================================
public:
//! @section Construction
    CmdArg( const QString& name = "", const QString& value = "") :
        name_(name),
        value_(value)
    {}
//<METHODS>
    QString name() const { return name_; }
    QString value() const { return value_; }

    bool isNull() {
        bool n = name_.isEmpty();
        bool v = value_.isEmpty();
        bool result = n && v;
        return result;
    }
//=============================================================================
protected:
//! @section Data
    QString name_;
    QString value_;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/base/CmdArgCol.cpp ----

CmdArgCol::CmdArgCol(const QString& args)
{
    QStringList divided;
    CmdArgCol_parser::splitCustom(args, divided);
    CmdArgCol_parser::createList(divided, *this);
}

//=============================================================================
int CmdArgCol::append(const QString& value, const QString& name)
{
    args_.append(CmdArg(name, value));
    return 1;
}

//=============================================================================
CmdArg CmdArgCol::get(size_t index)
{
    if (index >= count()) return CmdArg();
    return args_.at(index);
}

//=============================================================================
CmdArg CmdArgCol::get(const QString& name, const QString& implicitValue, size_t index)
{
    for ( int i = 0; i < args_.count(); i++ ) {
        CmdArg arg = get(i);
        if ( arg.name() != name) continue;
        if ( index-- == 0) return arg;
    }

    return CmdArg(name, implicitValue);
}

//=============================================================================
const CmdArg* CmdArgCol::getArg(const QString& name, int index) const
{
    for ( int i = 0; i < args_.count(); i++ ) {
        const CmdArg& argi = args_[i];
        if ( argi.name() != name) continue;
        if ( index-- == 0) return &argi;
    }

    return nullptr;
}

//=============================================================================
QString CmdArgCol::toString( EStyle style, int last )
{
    int from = 0;
    int to = count();

    if (last > 0) from = to-last;
    if (from < 0) from = 0;
    if (last < 0) to += last;
    if (to > count()) to = count();

    QString result;

    if ( style == AS_FORMATTED)
    {   for ( int i = from; i < to; i++) {
            CmdArg arg = get(i);
            if (i > 0) result += "<br/>";
            result += QString("&nbsp;&nbsp;&nbsp;&nbsp;[") + QString::number(i) + QString("] ");
            if (!arg.name().isEmpty()) result += arg.name();
            result += ":" + arg.value().replace(" ","&#9617;");
        }
    }
    else if (style == AS_JSON)
    {
        result = "[\n";
        for ( int i = from; i < to; i++) {
            CmdArg arg = get(i);
            result += QString("[\"") + arg.name()+ "\", \"" + arg.value() + "\"],\n";
        }
        result += "]";
    }
    else {
        for ( int i = from; i < to; i++) {
            CmdArg arg = get(i);
            if ( i > 0 ) result += " ";
            if (!arg.name().isEmpty()) {
                result += "--";
                result += arg.name();
                if (!arg.value().isEmpty() ) result += " ";
            }
            result += arg.value();
        }
    }

    return result;
}



// ---- FILE: /home/rce/KADLUB/cvz/base2/base/CmdArgCol.h ----


//=============================================================================
class  CmdArgCol {
//=============================================================================
public:
    enum EStyle { AS_NORMAL, AS_FORMATTED, AS_JSON };
//! @section Construction
    CmdArgCol() {};
    CmdArgCol(const QString& args);
//<METHODS>
    int append(const QString& value, const QString& name = "");
    int appendError(const QString& text) { return append(text, "ERROR"); }
    int appendWarning(const QString& text) { return append(text, "WARNING"); }
    size_t count() { return args_.count(); }
    CmdArg get(size_t index);
    CmdArg get(const QString& name, const QString& implicitValue = "", size_t index = 0);
    const CmdArg* getArg(const QString& name, int index = 0) const;

    //! @note last == 0 - all
    //! last positive - last arguments;
    //! last negative - without |last| at end
    QString toString( EStyle style = AS_NORMAL, int last = 0);

    int sizeOccupied() {
        int result = 0;
        for (CmdArg& arg: args_) { result+=arg.name().length()+arg.value().length(); }
        return result;
    }

    bool isNull() const { return args_.isEmpty(); }
//=============================================================================
protected:
//! @section Data
    QVector<CmdArg> args_;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/base/CmdArgCol_parser.cpp ----


//=============================================================================
void CmdArgCol_parser::splitCustomAppend(QStringList& result, QString& current, bool& isNoname)
{
    current = current.trimmed();
    if (current.isEmpty()) return;

    if (current.startsWith("--")) {
        isNoname = false;
        int spaceIndex = current.indexOf(' ');
        if (spaceIndex != -1) {
            result.append(current.left(spaceIndex));
            current = current.mid(spaceIndex+1);
        }
    }

    if (isNoname && current[0]!='{' && current.contains(' ')) {
        QStringList parts = current.split(' ');
        for (QString& part : parts) { if (!part.isEmpty()) result.append(part); }
    }
    else {
        result.append(current);
    }

    current = "";
}

bool CmdArgCol_parser::isJsonQuote_(const QChar& ch)
{
    if (ch=='"') return true;
    if (ch=='\'') return true;
    return false;
}

//=============================================================================
void CmdArgCol_parser::splitCustom(const QString &input, QStringList& result )
{
    QString     current;

    bool        inEscape   = false;
    bool        inQuotes   = false;
    int         braceDepth = 0;
    int         nameLevel  = 0;
    bool        inNoname   = true;

/*  replace only after detailed tests
    bool        inString   = false;
    QChar       inStringQuote = QChar::Null;
    QChar       previousInBraces = QChar::Null;*/

    for (int i = 0; i < input.size(); ++i) {
        QChar c = input[i];

        if (inEscape) {
            if (c == '\\') { current += '\\'; }
            else if (c == '"') { current += '"'; }
            else { current += '\\'; current += c; }

            inEscape = false;
            continue;
        }

        if (inQuotes) {
            if (c == '"') {
                splitCustomAppend(result, current, inNoname);
                inQuotes = false;
                continue;
            }
            current += c;
            continue;
        }

        if ( braceDepth > 0 ) {
            if      ( c == '}' ) braceDepth--;
            else if ( c == '{' ) braceDepth++;

            current += c;

            if ( braceDepth==0 ) { splitCustomAppend(result, current, inNoname); continue; }
            continue;
        }

/* replace only after detailed tests
        if ( braceDepth > 0 ) {
            if ( inString && previousInBraces!='\\' && current==inStringQuote) {
                inString = false;
                previousInBraces = QChar::Null;
            }
            else if ( !inString && isJsonQuote_(c)) {
                inString = true;
                inStringQuote = c;
            }

            if (!inString) {
                if      ( c == '}' ) braceDepth--;
                else if ( c == '{' ) braceDepth++;
            }

            previousInBraces = c;
            current += c;

            if ( braceDepth==0 ) {
                splitCustomAppend(result, current, inNoname);
                inString = false;
                continue;
            }
            continue;
        }*/

        if (c=='\\') { inEscape = true; continue; }

        if (nameLevel==0 && c==' ') nameLevel++;
        else if (nameLevel==1 && c=='-') nameLevel++;
        else if (nameLevel==2 && c=='-') nameLevel++;
        else { nameLevel=0; if (c==' ') nameLevel++; }

        if (nameLevel==3) {
            nameLevel = 0;
            if (current.chopped(1).trimmed().isEmpty()) {
                current.append(c);
                continue;
            }
            else {
                current.chop(1);
                splitCustomAppend(result, current, inNoname);
                current = "--";
                nameLevel = 0;
                continue;
            }
        }

        QString current0;

        if ( inQuotes || braceDepth>0 ) { current += c; continue; }
        else if ( c == '{' ) { current0 = '{'; braceDepth++; }
        else if ( c == '"' ) { current0 = '"'; inQuotes = true; }
        else                 { current += c;   continue; }

        if (!current.isEmpty()) {
            splitCustomAppend(result, current, inNoname);
            current = current0;
        }
    }

    if (!current.isEmpty()) {
        splitCustomAppend(result, current, inNoname);
    }
}

//=============================================================================
void CmdArgCol_parser::createList(QStringList& divided, CmdArgCol& args)
{
    QString name;
    QString value;

    while (!divided.isEmpty()) {
        QString current = divided.front();
        divided.pop_front();
        current = current.trimmed();

        if (current.left(2) == "--") {
            if (!name.isEmpty()) {
                args.append(value, name);
            }
            name = current.mid(2);
            value = "";
            continue;
        }

        if (current.startsWith("\"")) current = current.mid(1);

        if (name.isEmpty()) {
            args.append(current);
        }
        else {
            if (!value.isEmpty()) value += " ";
            value += current;
        }
    }

    if (!name.isEmpty()) {
        args.append(value, name);
    }
}

//=============================================================================
QString CmdArgCol_parser::afs_single_test(const QString testName, const QString& cmd) {
    QStringList divided;
    CmdArgCol_parser::splitCustom(cmd, divided);

    QStringList divided2 = divided;

    CmdArgCol args;
    CmdArgCol_parser::createList(divided, args);


    divided2.append("<br/>");
    QString dividedOut = divided2.join("<br/>");
    dividedOut.replace(" ", "&#9617;");

    QString result;
    QTextStream stream(&result);

    stream << "<br/>" << testName << "<b>: expected:</b> " << QString(cmd).replace(" ", "&#9617;")  << "<br/>"
        << dividedOut;

    result.append("<br/>");
    result.append(args.toString(CmdArgCol::AS_FORMATTED));

    return result;
}



// ---- FILE: /home/rce/KADLUB/cvz/base2/base/CmdArgCol_parser.h ----


class  CmdArgCol_parser {
//=============================================================================
public:
//! @section Construction
    CmdArgCol_parser() = delete;
//<METHODS>
    static void createList_old(const QString& args, CmdArgCol& argsCol);

    static void splitCustomAppend(QStringList& result, QString& current, bool& isNoname);
    static void splitCustom(const QString &input, QStringList& result );
    static void createList(QStringList& divided, CmdArgCol& args);

    static QString afs_single_test(const QString testName, const QString& cmd);
protected:
    static bool isJsonQuote_(const QChar& ch);
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys/Cmd.h ----

//=============================================================================
class  Cmd {
//=============================================================================
public:
//! @section Construction
    Cmd(const QString& name, CommandHandler handler, bool excludeExeRec = false, bool excludeUpdate = false) :
        name_(name),
        handler_(handler),
        excludeExeRec_(excludeExeRec),
        excludeUpdate_(excludeUpdate)
    {}
//<METHODS>
    int execute(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) {
        return handler_(args, data, context);
    }

    QString name() const    { return name_; }
    bool    excludeExeRec() { return excludeExeRec_; }
    bool    excludeUpdate() { return excludeUpdate_; }
//=============================================================================
protected:
//! @section Data
    QString        name_;
    CommandHandler handler_;
    bool           excludeExeRec_;
    bool           excludeUpdate_;
};


// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys/CmdContext.h ----


#define CONTEXT CmdContext::instance()

//=============================================================================
class  CmdContext final : public CmdContextIface {
//=============================================================================
public:
//! @section Construction
    CmdContext() = default;
    static QSharedPointer<CmdContext> instance()
    {   static QSharedPointer<CmdContext> i(new CmdContext()); return i; }
//<METHODS>
    void reply(QString cmd, QByteArray data) {};
//=============================================================================
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys/CmdContextDataIface.h ----


//=============================================================================
class  CmdContextDataIface {
//=============================================================================
public:
//! @section Construction
    virtual ~CmdContextDataIface() = default;
    virtual ClientContext* getAsClientContext() { return nullptr; }
    virtual bool hasClientId() { return true; }
    virtual int uniqueId() { return -1; }
//<METHODS>
//=============================================================================
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys/CmdContextIface.h ----


//=============================================================================
class  CmdContextIface {
//=============================================================================
public:
//! @section Construction
    virtual ~CmdContextIface() = default;
//<METHODS>
    virtual void reply(QString cmd, QByteArray data = QByteArray()) = 0;
    virtual void reply_threadSafe(QString cmd, QByteArray data = QByteArray()) { assert(0); }

    virtual ClientContext* getAsClientContext() { return nullptr; }

    virtual void forceDisconnect() {};

    virtual bool logCmd() { return false; }

    int uniqueId() {
        if (clientData_ == nullptr) return -1;
        return (clientData_->uniqueId());
    }

    CmdContextDataIface* clientData() {
        assert(QThread::currentThread() == QCoreApplication::instance()->thread());
        return clientData_; }
    void clientDataSet(CmdContextDataIface* data) {
        assert(QThread::currentThread() == QCoreApplication::instance()->thread());

        assert(clientData_ == nullptr);
        clientData_ = data; }
//=============================================================================

    CmdContextDataIface* clientData_ = nullptr;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys/CmdExeGuard.h ----


//=============================================================================
class  CmdExeGuard {
//=============================================================================
public:
enum Phase {
    Record = 0,
    Update = 100
};

//! @section Construction
CmdExeGuard(Phase phase = Record) : phase_(phase) {};
    virtual ~CmdExeGuard() = default;
//! @section Methods
    Phase phase() { return phase_; }
    virtual void commandExecuted() = 0;
//=============================================================================

    Phase phase_;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys/CmdSys.cpp ----


//=============================================================================
void CmdSys::add(const QString& name, CommandHandler handler, bool excludeExeRec, bool excludeUpdate )
{
    Cmd cmd(name, handler, excludeExeRec, excludeUpdate);
    cmds_.insert(name, cmd);
    timer_.start();
}

//=============================================================================
Cmd* CmdSys::get(const QString& name)
{
    auto it = cmds_.find(name);
    if (it == cmds_.end())  return nullptr;
    return &(*it);
}

//=============================================================================
int CmdSys::execute_(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context, const QString& sourceName, int sourceIndex)
{
    if ( mainThread_) {
        assert(QThread::currentThread()==QCoreApplication::instance()->thread());
    }
    if (QCoreApplication::closingDown()) return 0;

    //! @section - search command
    if (args.count() == 0) {
        return -1;
    }

    runningIndex_++;

    QString name = args.get(0).value();
    Cmd* cmd = get(name);
    //QString cmdString = args.toString();
    if (cmd == nullptr) {
        cmd = get("__not_implemented__");
        if (cmd == nullptr) return -1;
    }

    //! @section - execute
    int result = cmd->execute(args, data, context);

    //! @section - postprocess
    bool recordCmd = false;
    if (!cmd->excludeExeRec()) {
        recordCmd = true;
    }
    else if (!MonitorSocketCmd::instance().isEmpty()) {
        if ( MonitorSocketCmd::instance().registered(context->uniqueId()) ) recordCmd = true;
    }

    if (recordCmd) {
        QString argsStr = args.toString();
    }

    if (!cmd->excludeUpdate()) {
        int i = 0;
        for (int i = 0; i < guards_.size(); ++i) {
            guards_[i]->commandExecuted();
        }
    }

    return result;
}

//=============================================================================
void CmdSys::execute_threadSafe(const QString& args, const QString& sourceName, int sourceIndex)
{
    QMetaObject::invokeMethod(this, [this, args, sourceName, sourceIndex]() {
        execute(args, sourceName, sourceIndex);
    }, Qt::QueuedConnection);
}

//=============================================================================
int CmdSys::execute(const QString& args, const QString& sourceName, int sourceIndex)
{
    Q_ASSERT(QThread::currentThread() == this->thread());

    if (!executingArgs_.isNull()) {
        execute_threadSafe(args, sourceName, sourceIndex);
        return 0;
    }

    executingArgs_ = CmdArgCol(args);
    auto guard = qScopeGuard([this] { executingArgs_ = CmdArgCol(); });

    return execute_(executingArgs_, nullptr, CONTEXT, sourceName, sourceIndex);
}



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys/CmdSys.h ----


#define CMD_SYS  CmdSys::instance()
#define CMD_ARGS ( CmdArgCol&, QByteArray*, const QSharedPointer<CmdContextIface>& )
#define CMD_ARGS_U ( [[maybe_unused]] CmdArgCol& args, [[maybe_unused]] QByteArray* data, [[maybe_unused]] const QSharedPointer<CmdContextIface>& context )

//=============================================================================
class  CmdSys : public QObject {
//=============================================================================
    Q_OBJECT
public:
//! @section Construction
    CmdSys() : QObject(nullptr) {};
    static CmdSys& instance() { static CmdSys i; return i; }
//! @section Neighbours
    //CmdExeRecCol& exeRecord() { return CmdExeRecCol::instance(); };
//<METHODS>
    int execute(const QString& args, const QString& sourceName = "", int sourceIndex = -1);
    void execute_threadSafe(const QString& args, const QString& sourceName = "", int sourceIndex = -1);

    void add(const QString& name, CommandHandler handler, bool excludeExeRec = false, bool excludeUpdate = false);
    Cmd* get(const QString& mame);
    void reg(CmdExeGuard* guard) {
        if (guard->phase() == CmdExeGuard::Record) {
            guards_.prepend(guard);
            return;
        }
        guards_.append(guard);
    }
    void setMainThread(bool value) { mainThread_ = value; }
//=============================================================================
protected:
//! @section Data
    QMap<QString, Cmd>    cmds_;
    QVector<CmdExeGuard*> guards_;
    CmdArgCol             executingArgs_;
    int                   runningIndex_ = -1;
    QElapsedTimer         timer_;
    bool                  mainThread_ = true;
//! @section Internals
    int execute_(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context,
                const QString& sourceName, int sourceIndex);

    friend class Cmds_cmd_sys;
    friend class Interactive_uiControl;
    friend class CmdExeRecCol;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys/Cmds_cmd_sys.h ----


class  Cmds_cmd_sys {
//=============================================================================
public:
//! @section Construction
    Cmds_cmd_sys() = delete;
    static bool registerCmds_()
    {
        CMD_SYS.add("voidcmd", voidcmd );
        CMD_SYS.add("logcmd",  logcmd );

        CMD_SYS.add("__not_implemented__",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

            QString name = args.get(0).value();
            args.appendError(QString("command not implemented: ") + name);
            return -1;
        });

        return true;
    }

//! @section Command
    static int voidcmd( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int logcmd ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );

};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/CmdExeRec.h ----


//=============================================================================
class  CmdExeRec {
//=============================================================================
public:
//! @section Construction
    CmdExeRec( bool found, const QString& argsIn, const QString& argsInOut,
               int index, const QString& sourceName, int sourceIndex) :
        argsIn(argsIn),
        argsOut(argsInOut.mid(argsIn.length())),
        index(index),
        found(found),
        sourceName(sourceName),
        sourceIndex(sourceIndex) {};
//! @section Data
    QString  argsIn;
    QString  argsOut;
    int      index;
    bool     found = true;
    QString  sourceName;
    int      sourceIndex;
//=============================================================================
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/CmdExeRecCol.cpp ----


//=============================================================================
CmdExeRecCol::CmdExeRecCol() :
    CmdExeGuard(Record)
{
    CMD_SYS.reg(this);
}

//=============================================================================
void CmdExeRecCol::commandExecuted()
{
    CmdSys& cmdsys = CMD_SYS;

    QString args = CMD_SYS.executingArgs_.toString();

    add( true, args, args, CMD_SYS.runningIndex_, "", -1);
}



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/CmdExeRecCol.h ----


//=============================================================================
class  CmdExeRecCol : public CmdExeGuard {
//=============================================================================
public:
//! @section Construction
    CmdExeRecCol();
    static CmdExeRecCol& instance() { static CmdExeRecCol i; return i; }
//! @section Data
    void commandExecuted() override;
    void add(bool found, const QString& argsIn, const QString& argsInOut,
             int index, const QString& sourceName, int sourceId) {
        if (cache_) {
            recordsCache_.append(CmdExeRec(found, argsIn, argsInOut, index, sourceName, sourceId));
        }
        else {
            records_.append(CmdExeRec(found, argsIn, argsInOut, index, sourceName, sourceId));
        }
    }
    int count() const { return records_.count(); }
    CmdExeRec& get(int index) { return records_[index]; }
    CmdExeRec& last() { assert(count() > 0); return records_.last(); }

    void cacheSet() { cache_ = !cache_; records_.append(recordsCache_); recordsCache_.clear(); }
//=============================================================================
protected:
//! @section Data
    bool cache_ = false;

    QList<CmdExeRec> records_;
    QList<CmdExeRec> recordsCache_;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/Cmds_exerec.cpp ----


//=============================================================================
int Cmds_exerec::exerec_remove_filterout_command(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    CmdArg arg1 = args.get(1);
    if (arg1.value().isEmpty()) return 0;

    Model_filteroutCmds::instance().cmdRemove(arg1.value());

    //QString comp = filteroutCommands_codeSegment();
    //ExerecCodeSegments::instance().qmlExcludedCommandsSet(comp);

    ExerecModelProxy::instance().invalidateFilterOverride();

    return 0;
}

//=============================================================================
int Cmds_exerec::exerec_add_filterout_command(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    CmdArg arg1 = args.get(1);
    if (arg1.value().isEmpty()) return 0;

    Model_filteroutCmds::instance().cmdAdd(arg1.value());

    /*QString comp = filteroutCommands_codeSegment();
    ExerecCodeSegments::instance().qmlExcludedCommandsSet(comp);*/

    ExerecModelProxy::instance().invalidateFilterOverride();

    return 0;
}

//=============================================================================
int Cmds_exerec::cmds_stop_record(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    CmdExeRecCol::instance().cacheSet();
    return 0;
}



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/Cmds_exerec.h ----


class  Cmds_exerec {
//=============================================================================
public:
//! @section Construction
    Cmds_exerec() = delete;
    static bool registerCmds_()
    {
        CMD_SYS.add("exerec_add_filterout_command",     exerec_add_filterout_command );
        CMD_SYS.add("exerec_remove_filterout_command",  exerec_remove_filterout_command );
        CMD_SYS.add("cmds_stop_record",  cmds_stop_record );

        return true;
    }
//! @section Command
protected:
    static int exerec_add_filterout_command  ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int exerec_remove_filterout_command  ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int cmds_stop_record ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );

    friend class ExerecModelProxy;
    friend class Cmds_active;
    friend class Cmds_ui_model_refresh;
    friend class UiControl;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/ExerecData.h ----


class  ExerecData
{
    Q_GADGET
    Q_PROPERTY(QString cmd  MEMBER cmd )
    Q_PROPERTY(QString args MEMBER args )

public:
    ExerecData() {}

    QString cmd  = "__UNDEFINED__";
    QString args = "__UNDEFINED__";
};

Q_DECLARE_METATYPE(ExerecData)



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/ExerecModel.cpp ----


//=============================================================================
ExerecModel::ExerecModel() :
    CmdExeGuard(Update)
{
    CMD_SYS.reg(this);
}

//=============================================================================
void ExerecModel::commandExecuted()
{
    CmdSys& cmdsys = CMD_SYS;
    CmdExeRecCol& reccol = CmdExeRecCol::instance();
    int sysCount = CmdExeRecCol::instance().count();

    if ( sysCount > countIncluded_ ) {
        beginInsertRows(QModelIndex(), countIncluded_, sysCount-1);
        countIncluded_ = sysCount;
        endInsertRows();

        ExerecModelProxy::instance().sort(0, Qt::AscendingOrder);
    }
}

//=============================================================================
QVariant ExerecModel::data(const QModelIndex &modelIndex, int role) const
{
    if (!modelIndex.isValid()) return QVariant();

    CmdExeRecCol& exeRecord = CmdExeRecCol::instance();
    int row = modelIndex.row();
    CmdExeRec& exeRec = exeRecord.get(row);

    if (role == cmdName) {
        QString cmd = exeRec.argsIn.trimmed().split(" ").first();
        return QVariant::fromValue(cmd);
    }
    if (role == exerecData) {
        ExerecData data;
        data.args = "";
        if (!exeRec.sourceName.isEmpty()) {
            data.args += QString() + "source: " + exeRec.sourceName +
                         " [" + QString::number(exeRec.index) + "]<br/>";
        }
        data.cmd  = exeRec.argsIn.trimmed().split(" ").first();
        data.args += "[" + QString::number(exeRec.index) + "] ";
        QString in = exeRec.argsIn;
        int spaceIndex = in.indexOf(' ');
        if (spaceIndex != -1)in.insert(spaceIndex, "</b><br/>");
        data.args += "<b>";
        if (exeRec.argsIn.length() < 320) {
            data.args += in;
        }
        else {
            data.args += in.left(240);
            data.args += QString("<span style='font-size:6pt;'>") + in.mid(240) + "</span>";
        }
        if (!exeRec.argsOut.isEmpty()) data.args += QString("<br/>") + exeRec.argsOut;
        return QVariant::fromValue(data);
    }

   return QVariant();
}



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/ExerecModel.h ----


//=============================================================================
class  ExerecModel: public QAbstractListModel, public CmdExeGuard {
//=============================================================================
public:
//! @section Enums
    enum EExerecModelRoles { exerecData = Qt::UserRole + 1, cmdName };
//! @section Construction
    ExerecModel();
    static ExerecModel& instance() { static ExerecModel i; return i; }
//<METHODS>
//! @section Overrides
    void commandExecuted() override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override { return countIncluded_; };
    QVariant data(const QModelIndex &modelIndex, int role) const override;
    QHash<int, QByteArray> roleNames() const override { return {{exerecData, "exerecData"}, {cmdName, "cmdName"}}; }
//=============================================================================
protected:
//! @section Data
    int countIncluded_ = 0;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/ExerecModelProxy.h ----


//=============================================================================
class  ExerecModelProxy: public QSortFilterProxyModel {
//=============================================================================
public:
//! @section Construction
    ExerecModelProxy() {
        setSourceModel(&ExerecModel::instance());
    }
    static ExerecModelProxy& instance() { static ExerecModelProxy i; return i; }
//! @section Methods
//! @section Overrides
    bool lessThan(const QModelIndex &top, const QModelIndex &bottom) const override {
        return top.row() > bottom.row();
    }
    void invalidateFilterOverride() { QSortFilterProxyModel::invalidateFilter(); }
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
            // Get the index for the column you want to check (e.g., column 0)
        QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
        QString value = index.data(ExerecModel::EExerecModelRoles::cmdName).toString();
        bool result = !Model_filteroutCmds::instance().contains(value);
        return result;
    }
//=============================================================================
protected:
//! @section Data
    friend class Cmds_exerec;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/GenericApp.qml ----


Rectangle  {
    id: root;
    color: "#D9D9D9";
    anchors.fill: parent;

    function activateTab(tabName) {
        tabCmdlineEx.visible   = (tabCmdlineEx.objectName    === tabName) ? true : false;
        tabInteractive.visible = (tabInteractive.objectName    === tabName) ? true : false;
        tabAbout.visible       = (tabAbout.objectName   === tabName) ? true : false;
    }

    Rectangle { id: mainSpace; color: "#4cc4bc"
        x: 0; y: 0; width: root.width; height: 40

        Image {x:2; y:2; width:32; height:32; source:"images/icon.svg"}
        Text {x:40; y: 8; font.pointSize: 14; font.bold: true; text: "PROMPT Assembler" }

        Button { id: buttonCmdlineEx;   x:260;       y:10; width: 110; height: 30;
                 text: "Executed"; onClicked: activateTab("tabCmdlineEx") }
        Button { id: buttonInteractive; x:260+120;   y:10; width: 110; height: 30;
                 text: "Cmdline";  onClicked: activateTab("tabInteractive") }
        Button { id: buttonAbout;       x:260+120*2; y:10; width: 110; height: 30;
                 text: "About";    onClicked: {activateTab("tabAbout") } }
    }

    Rectangle { objectName: "tabCmdlineEx"; id: tabCmdlineEx
        visible: false
        color: "#F0F0F0"
        x: 10; y: 50; width: root.width-20; height: root.height-60

        Button { x:10; y: 10; text: "STOP CMDS REC";
            onClicked: { interactiveIface.callCmd("cmds_stop_record"); }
        }

        CmdsExecuted {
            x: 10;
            y: 40;
            width: root.width-20;
            height: root.height-170
        }
    }

    Interactive { objectName: "tabInteractive"; id: tabInteractive
        visible: true;
        x: 10; y: 50; width: root.width-20; height: root.height-60
    }

    Rectangle { objectName: "tabAbout"; id: tabAbout

        visible: false;
        x: 10; y: 50; width: root.width-20; height: root.height-60

        Text {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width - 20
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            text: "<p><b>PROMPT Assembler</b></p>" +
            "<p>Copyright 2025 Pert Talla. All right reserved.</p>"
        }
    }
}



// ---- FILE: /home/rce/KADLUB/cvz/base2/cmd_sys_display/Model_filteroutCmds.h ----


//=============================================================================
class  Model_filteroutCmds: public QAbstractListModel {
//=============================================================================
public:
//! @section Enums
    enum EExerecModelRoles { cmd = Qt::UserRole + 1 };
//! @section Construction
    Model_filteroutCmds() = default;
    static Model_filteroutCmds& instance()        { static Model_filteroutCmds i; return i; }

//! @section Methods
    QStringList cmdsExcluded() { return cmds_; }

    bool contains(const QString& cmd) {
        return cmds_.contains(cmd);
    }

    void cmdAdd(const QString& cmd) {
        if (!monitor_ && contains(cmd)) return;

        int newRowIndex = cmds_.count();
        beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
        cmds_.append(cmd);
        endInsertRows();

        if (monitor_) {
            qDebug() << "monitor add";
        }
    }

    void cmdRemove(const QString& cmd) {
        int rowIndex = cmds_.indexOf(cmd);
        if (rowIndex == -1) return;

        beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
        cmds_.removeAt(rowIndex);
        endRemoveRows();
    }

//<METHODS>
//! @section Overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (monitor_) {
            qDebug() << "monitor count";
        }
        return cmds_.count();
    };
    QVariant data(const QModelIndex &modelIndex, int role) const override {
        if (monitor_) {
            qDebug() << "monitor DATA";
        }

        if (!modelIndex.isValid()) return QVariant("mistake");

        if (role == cmd) { return QVariant(cmds_.at(modelIndex.row())); }

        return QVariant("mistake");;
    }
    QHash<int, QByteArray> roleNames() const override { return {{cmd, "cmd"}}; }
//=============================================================================
protected:
//! @section Data
    QStringList cmds_;
    bool monitor_ = false;

    friend class Model_controls;
};


// ---- FILE: /home/rce/KADLUB/cvz/base2/code_analyzer/AnalyzerCode.h ----


class  AnalyzerCode
{
public:
    //=========================================================================
    static QStringList getFiles(QDir& dir, QStringList filters) {
        QDirIterator it(
            dir.absolutePath(),
            filters,
            QDir::Files | QDir::NoSymLinks,
            QDirIterator::Subdirectories
            );

        QStringList files;

        while (it.hasNext()) files << it.next();

        return files;
    }

    //=========================================================================
    static QString resultFile(const QString& dirPath)
    {
        QDir dir(dirPath);

        QString dirName = dir.dirName();
        if (dirName.isEmpty()) dirName = "mismatched.h";

        return dir.absoluteFilePath(dirName + ".h");
    }

    //=========================================================================
    static QByteArray extractView(const QByteArray& input)
    {
        QByteArray result;
        QList<QByteArray> lines = input.split('\n');

        bool inBlock = false;

        for (const QByteArray& line : lines) {

            if (line.contains("@" \
                              "view:beg")) {
                inBlock = true;
                continue;
            }

            if (line.contains("@" \
                              "view:end")) {
                inBlock = false;
                continue;
            }

            int exportPos = line.indexOf("@" \
                                         "view:export");
            if (exportPos >= 0) {
                result.append(line.left(exportPos).trimmed());
                result.append('\n');
                continue;
            }

            if (inBlock) {
                if (line.contains("@" \
                                  "view:exclude")) continue;

                result.append(line);
                result.append('\n');
            }
        }

        return result;
    }

    //=========================================================================
    static void composeToFile (
        const QStringList& files, QFile& outFile,
        bool useViews = false )
    {
        for (const QString& path : files) {
            QFile f(path);
            if (!f.open(QIODevice::ReadOnly)) {
                CMD_SYS.execute( QString() +
                                "logcmd --ERROR AnalyzerCode::composeToFile cannot open " +
                                path );
                continue;
            }

            QByteArray content = f.readAll();
            f.close();

            if (useViews)
                content = extractView(content);

            if (content.isEmpty())
                continue;

            outFile.write("\n// ---- FILE: ");
            outFile.write(path.toUtf8());
            outFile.write(" ----\n\n");

            outFile.write(content);
            outFile.write("\n");

            f.close();
        }
    }

    //=============================================================================
    static QStringList extractIncludes(const QString& filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return {};

        const bool isQml = filePath.endsWith(".qml", Qt::CaseInsensitive);

        QStringList result;
        QTextStream in(&file);

        while (!in.atEnd()) {
            const QString line = in.readLine().trimmed();

            if (isQml) {
                // QML: include může být kdekoliv (typicky v komentáři)
                if (!line.contains("#include"))
                    continue;
            } else {
                // C/C++: musí začínat na #include (po trimu)
                if (!line.trimmed().startsWith("#include"))
                    continue;
            }

            int firstQuote = line.indexOf('"');
            if (firstQuote < 0)
                continue;

            int secondQuote = line.indexOf('"', firstQuote + 1);
            if (secondQuote < 0)
                continue;

            const QString includeFile = line.mid(firstQuote + 1, secondQuote - firstQuote - 1);

            if (!includeFile.isEmpty()) result.append(includeFile);
        }

        return result;
    }

    //=============================================================================
protected:
    //! @section Data
    int arg_;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/code_analyzer/AnalyzerConnector.h ----


class AnalyzerConnector
//=============================================================================
{
public:
//! @section Construction
    AnalyzerConnector(const QString& node1, const QString& node2) :
        node1_(node1),
        node2_(node2)
    {
    }

    //! @section Accessors
    const QString& node1() const { return node1_; }
    const QString& node2() const { return node2_; }

    //node1-node2
    static QString signature(const QString& node1, const QString& node2)
    {
            return node1 + "-" + node2;
    }

    QString signature() const { return signature(node1_, node2_); }

    //=============================================================================
protected:
//! @section Data
    QString node1_;
    QString node2_;
};




// ---- FILE: /home/rce/KADLUB/cvz/base2/code_analyzer/AnalyzerNet.h ----


class AnalyzerNet
//=============================================================================
{
public:
    /// @section Construction
    AnalyzerNet() = default;

/// @section Methods
    //=================================
    bool add(const QString& node1, const QString& node2)
    {
        const QString sig = AnalyzerConnector::signature(node1, node2);

        if (connectors_.contains(sig))
            return false;

        connectors_.insert(sig, AnalyzerConnector(node1, node2));
        return true;
    }

    //=================================
    void addIncludesFromFile(const QString& filePath)
    {
        QString log("logcmd ");

        const QString from = AnalyzerNode::nameFromFilePath(filePath);

        log += from + " -> ";

        const QStringList includes = AnalyzerCode::extractIncludes(filePath);

        log += includes.join(" ");

        for (const QString& inc : includes) {
            const QString to = QFileInfo(inc).completeBaseName();
            if (from == to) continue;
            if (!to.isEmpty()) add(from, to);
        }

        log += " -> " + QString::number(connectors_.size());
        CMD_SYS.execute(log);
    }

//=============================================================================
protected:
/// @section Internals
    //=================================
    QString toDot_() const
    {
        QString out;
        QTextStream s(&out);

        for (auto it = connectors_.cbegin(); it != connectors_.cend(); ++it) {
            const AnalyzerConnector& c = it.value();
            s << "  \"" << c.node1() << "\" -> \"" << c.node2() << "\";\n";
        }

        return out;
    }

/// @section Data
    QMap<QString, AnalyzerConnector> connectors_;

/// @section friends
    friend class AnalyzerSys;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/code_analyzer/AnalyzerNode.h ----


class AnalyzerNode
{
public:
    /// @section Construction
    explicit AnalyzerNode(const QString& filePath, const QString& moduleName) :
        module_(moduleName)
    {
        dir_  = QFileInfo(filePath).absolutePath();
        name_ = nameFromFilePath(filePath);

        insertSuffix(filePath);
    }

    void insertSuffix(const QString& filePath)
    {
        QFileInfo fi(filePath);
        const QString suffix = fi.suffix().toLower();
        if (!suffix.isEmpty()) {
            extensions_.insert(suffix);
        }
    }

    /// @section Methods
    QString dir()    const { return dir_;  }
    QString name()   const { return name_; }
    QString module() const { return module_; }
    const QSet<QString>& extensions() const { return extensions_; }

    static QString nameFromFilePath(const QString& filePath)
    {
        QFileInfo fi(filePath);
        return fi.completeBaseName();
    }

    QString toStringDebug() const
    {
        if (extensions_.isEmpty()) {
            return name_;
        }

        QStringList exts = extensions_.values();
        exts.sort();

        return name_ + " [" + exts.join(" ") + "]";
    }

    QString label() const
    {
        QString base = name_;
        if (extensions_.isEmpty())
            return base;

        QStringList exts = extensions_.values();
        exts.sort();
        return base + "\\n[" + exts.join(" ") + "]";
    }

    QString LabelCmd() const
    {
        QString result;
        QTextStream s(&result);

        const QString filePath = dir_ + "/" + name_ + ".h";
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return label();
        QTextStream in(&file);

        const QString marker = QStringLiteral("CMD_SYS.add(\"");

        QStringList cmds;

        while (!in.atEnd()) {
            const QString line = in.readLine();

            int pos = line.indexOf(marker);
            if (pos < 0)
                continue;

            pos += marker.length();
            int end = line.indexOf('"', pos);
            if (end < 0)
                continue;

            cmds.append(line.mid(pos, end - pos));
        }

        s << label();
        s << "\\n-----------------------------";

        for (QString cmd : cmds) {
            s << "\\n" << cmd;
        }

        return result;
    }

    QStringList commands() const
    {
        QStringList result;

        // jen hlavičky
        if (!extensions_.contains("h"))
            return result;

        const QString path = dir_ + "/" + name_ + ".h";

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return result;

        QTextStream in(&file);

        while (!in.atEnd()) {
            const QString line = in.readLine();

            int posCmdSys = line.indexOf("CMD_SYS");
            if (posCmdSys < 0)
                continue;

            int firstQuote = line.indexOf('"', posCmdSys);
            if (firstQuote < 0)
                continue;

            int secondQuote = line.indexOf('"', firstQuote + 1);
            if (secondQuote < 0)
                continue;

            const QString cmd =
                line.mid(firstQuote + 1, secondQuote - firstQuote - 1);

            if (!cmd.isEmpty())
                result.append(cmd);
        }

        return result;
    }

private:
    QString       dir_;
    QString       name_;
    QString module_;

    QSet<QString> extensions_;

friend class AnalyzerNet;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/code_analyzer/AnalyzerSys.h ----


class AnalyzerSys
//=============================================================================
{
public:
/// @section Construction
    AnalyzerSys() = default;
/// @section Neighbours
    AnalyzerNet& net() { return net_; }

/// @section Methods
    //=================================
    void add(const QString& filePath, const QString& moduleName)
    {
        const QString name = AnalyzerNode::nameFromFilePath(filePath);

        auto it = nodes_.find(name);
        if (it == nodes_.end()) {
            nodes_.insert(name, AnalyzerNode(filePath, moduleName));
        }
        else {
            it->insertSuffix(filePath);
        }

        net_.addIncludesFromFile(filePath);
    }

    //=================================
    QString toStringDebug() const
    {
        QStringList parts;

        for (auto it = nodes_.cbegin(); it != nodes_.cend(); ++it) {
            parts.append(it.value().toStringDebug());
        }

        return parts.join(" ");
    }

    //=================================
    QString toDot() const
    {
        QString out;
        QTextStream s(&out);

        s << "digraph Analyzer {\n";
        s << toDot_();
        s << net_.toDot_();
        s << "}\n";

        return out;
    }

//=============================================================================
protected:
    /// @section Internals
    //=================================
    QString label_ (const AnalyzerNode* n, const QString& prefix = "    ") const {
        QString out;
        QTextStream s(&out);

        QString attrs;

        // QML → šedý obdélník
        if (n->extensions().contains("qml")) {
            attrs =
                " shape=box"
                " style=filled"
                " fillcolor=\"#DADADA\"";
        }

        // *Model* → obdélník
        if (n->name().contains("Model")) {
            attrs += ", shape=box"
            ", style=filled"
            ", fillcolor=\"#6FA3D8\"";   // ocelova modra
        }

        // Cmd_* → obdélník
        if (n->name().startsWith("Cmds_")) {
            attrs += ", shape=box"
                     ", style=filled"
                     ", fillcolor=\"#E6C1D8\"";   // light violet-red
        }

        s << prefix << "\"" << n->name() << "\" ";

        QString lbl;
        if (n->name().startsWith("Cmds_")) {
            lbl = n->LabelCmd();
        }
        else {
            lbl = n->label();
        }

        s << "[label=\"" << lbl << "\"" << attrs << "];\n";

        return out;
    }
    //=================================
    QString toDot_() const
    {
        QString out;
        QTextStream s(&out);

        QMap<QString, QList<const AnalyzerNode*>> groups;

        for (auto it = nodes_.cbegin(); it != nodes_.cend(); ++it) {
            const AnalyzerNode& node = it.value();

            if (node.name() == "main" || node.name() == "Main") {
                s << label_(&it.value(), "");
                continue;
            }

            groups[node.module()].append(&node);
        }

        for (auto it = groups.cbegin(); it != groups.cend(); ++it) {

            const QString& module = it.key();
            const QList<const AnalyzerNode*>& nodes = it.value();

            s << "  subgraph cluster_" << module << " {\n";
            s << "    label=\"" << module << "\";\n";

            for (const AnalyzerNode* n : nodes) s << label_(n);

            s << "  }\n";
        }

        return out;
    }


/// @section Data
    QMap<QString, AnalyzerNode> nodes_;
    AnalyzerNet                 net_;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/code_analyzer/Cmds_code_analyzer.h ----

class  Cmds_code_analyzer {
    CMD_SYS.add("dir_merge_files",
    [](CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) -> int {

        int result = 0;

        if (dirs_.isEmpty()) return args.appendError("no dir to solve");

        bool useViews =
            (args.get("views", "__UNDEF__").value() != "__UNDEF__");

        QStringList files;

        for (QString& dirStr : dirs_) {
            QDir dir(dirStr);
            if (!dir.exists()) result += args.appendError("directory does not exist");

            files << AnalyzerCode::getFiles(dir, QStringList() << "*.cpp" << "*.h" << "*.qml");
        }

        QDir dir(dirs_.first());

        if (files.isEmpty()) return args.appendError("dir_merge_files: no source files found");
        files.sort();

        if (dir.isRoot()) return args.appendError("dir_merge_files: root not possible");

        QString resultFileName = QDir::cleanPath(dirs_.first()) + ".h";
        QFile outFile(resultFileName);

        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return args.appendError("dir_merge_files: cannot open output file");

        AnalyzerCode::composeToFile(files, outFile, useViews);

        outFile.close();

        args.append(resultFileName, "RESULT_FILE");


        dir.cdUp();
        args.append("<a href='system_open_path " +
                    dir.absolutePath() +
                    "'>[OPEN DIR]</a>");

        args.append(
            QString("<a href='file_to_clipboard ") + resultFileName +
            "'>[COPY TO CLIPBOARD]</a>");

        return result;
    });
    CMD_SYS.add("dir_load_net_debug",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
        const QString debugStr = sys_.toStringDebug();
        args.append(debugStr, "DEBUG");
        return 0;
    });
    CMD_SYS.add("dir_export_dot",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (dirs_.isEmpty())
            return args.appendError("no dir to analyze");

        // stejný průchod jako dir_load_net / dir_merge_files
        for (const QString& dirStr : dirs_) {
            QDir moduleDir(dirStr);
            if (!moduleDir.exists()) {
                args.appendError("directory does not exist: " + dirStr);
                continue;
            }

            QStringList moduleFiles =
            AnalyzerCode::getFiles(
                moduleDir,
                QStringList() << "*.h" << "*.cpp" << "*.qml"
                );

            const QString moduleName = moduleDir.dirName();

            // naplnění uzlů + hran
            for (const QString& filePath : moduleFiles) {
                sys_.add(filePath, moduleName);
            }
        }

        // výstupní soubor – stejný základ jako dir_merge_files, ale .dot
        QDir dir(dirs_.first());
        if (dir.isRoot())
            return args.appendError("dir_export_dot: root not possible");

        QString base = QDir::cleanPath(dirs_.first());
        QString dotFileName = base + ".dot";

        QFile outFile(dotFileName);
        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return args.appendError("dir_export_dot: cannot open output file");

        QByteArray dot = sys_.toDot().toUtf8();
        outFile.write(dot);
        outFile.close();

        args.append(dotFileName, "RESULT_DOT");

        // otevření adresáře s výsledným .dot souborem
        dir.cdUp();
        args.append("<a href='system_open_path " +
                    dir.absolutePath() +
                    "'>[OPEN DIR]</a>");
        args.append("<a href='system_dot_to_svg " +
                    dotFileName +
                    "'>[DOT-&gtSVG]</a>");

        return 0;
    });

    }
//=============================================================================
protected:
    /// @section Data
    inline static AnalyzerSys sys_;
    inline static QStringList dirs_ = QStringList()
        << "../../../base2/base/"
        << "../../../base2/cmd_sys"
        << "../../../base2/cmd_sys_display"
        << "../../../base2/code_analyzer"
        << "../../../apky/PROMPT_ASSEMBLER"
        << "../../../base2/utility";

    friend class Cmds_code_analyzer_test;
};


// ---- FILE: /home/rce/KADLUB/cvz/base2/code_analyzer/Cmds_code_analyzer_test.h ----

class  Cmds_code_analyzer_test {
//=============================================================================
public:
//! @section Construction
    Cmds_code_analyzer_test() = delete;
//<METHODS>
    static void registerCmds_() {
    CMD_SYS.add("dir_list",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (Cmds_code_analyzer::dirs_.isEmpty())
            return args.appendWarning("no dirs configured");

        QStringList uniqueDirs;

        for (const QString& dirStr : Cmds_code_analyzer::dirs_) {
            if (!uniqueDirs.contains(dirStr))
                uniqueDirs.append(dirStr);
        }

        args.append(QString("<br/>"), "NL");

        for (const QString& d : uniqueDirs) {
            args.append(d + "<br/>", "DIR");
        }

        return 0;
    });
    CMD_SYS.add("select_dir",
    [](CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) -> int {
        Q_UNUSED(data) Q_UNUSED(context) Q_UNUSED(args)

        return 0;
    });
    CMD_SYS.add("pass_dir",
    [](CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) -> int {
        Q_UNUSED(data) Q_UNUSED(context)

        int result = 0;

        if (Cmds_code_analyzer::dirs_.isEmpty()) return args.appendError("no dir to solve");

        QDir dir(Cmds_code_analyzer::dirs_.first());
        QStringList files = AnalyzerCode::getFiles(dir, QStringList()<<"*.pro");

        for (QString& file: files) {
            result += args.append(file, "FILE");
        }

        return result;
    });
    CMD_SYS.add("dir_list_includes_test",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (Cmds_code_analyzer::dirs_.isEmpty())
            return args.appendError("no dir to analyze");

        QStringList files;

        // stejná logika jako dir_load_net
        for (const QString& dirStr : Cmds_code_analyzer::dirs_) {
            QDir dir(dirStr);
            if (!dir.exists()) {
                args.appendError("directory does not exist: " + dirStr);
                continue;
            }

            files << AnalyzerCode::getFiles(
                dir,
                QStringList() << "*.h" << "*.cpp" << "*.qml"
                );
        }

        if (files.isEmpty())
            return args.appendWarning("no source files found");

        args.append("<br/>", "NL");

        for (const QString& filePath : files) {

            QStringList includes =
                AnalyzerCode::extractIncludes(filePath);

            if (includes.isEmpty())
                continue;

            QString out;
            QTextStream s(&out);

            s << QFileInfo(filePath).fileName() << " ->";

            for (const QString& inc : includes)
                s << inc << " ";

            args.append(out.trimmed() + "<br/>", "FILE");
        }

        return 0;
    });

    }
};


// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/Cmds_utility_system.h ----


//=============================================================================
class  Cmds_utility_system {
//=============================================================================
public:
//! @section Construction
    Cmds_utility_system() = delete;
//<METHODS>
    static void registerCmds() {
    CMD_SYS.add("system_open_path",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (args.count() < 2)
            return args.appendError("open_path: missing path");

        CmdArg arg1 = args.get(1);
        const QString path = arg1.value();

        QFileInfo fi(path);
        if (!fi.exists())
            return args.appendError("open_path: path does not exist: " + path);

        QUrl url = QUrl::fromLocalFile(fi.absoluteFilePath());

        if (!QDesktopServices::openUrl(url))
            return args.appendError("open_path: failed to open: " + path);

        return 0;
    });
    CMD_SYS.add("system_dot_to_svg",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

                    int result = 0;

        if (args.count() < 2)
            return args.appendError("system_dot_to_svg: missing .dot file");

        CmdArg arg1 = args.get(1);
        const QString dotPath = arg1.value();

        QFileInfo fi(dotPath);
        if (!fi.exists())
            return args.appendError("system_dot_to_svg: file does not exist: " + dotPath);

        if (fi.suffix().toLower() != "dot")
            return args.appendError("system_dot_to_svg: not a .dot file: " + dotPath);

        const QString svgPath =
            fi.absolutePath() + "/" + fi.completeBaseName() + ".svg";

        QProcess proc;
        QStringList procArgs;
        procArgs << "-v"
                 << "-Tsvg" << fi.absoluteFilePath()
                 << "-o"    << svgPath;

        proc.start("dot", procArgs);

        if (!proc.waitForFinished())
            return args.appendError("system_dot_to_svg: dot execution failed");

        if (proc.exitStatus() != QProcess::NormalExit || proc.exitCode() != 0)
            result += args.appendError("system_dot_to_svg: dot returned error");

        result += args.append(svgPath, "RESULT_SVG");

        result += args.append("<a href='system_open_path " +
                    svgPath +
                    "'>[OPEN SVG]</a>");

        return result;
    });
    CMD_SYS.add("file_to_clipboard",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (args.count() < 2)
            return args.appendError("file_to_clipboard: missing file path");

        const QString filePath = args.get(1).value();
        if (filePath.isEmpty())
            return args.appendError("file_to_clipboard: empty file path");

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return args.appendError("file_to_clipboard: cannot open file " + filePath);

        const QByteArray content = file.readAll();
        file.close();

        QClipboard* clipboard = QGuiApplication::clipboard();
        if (!clipboard)
            return args.appendError("file_to_clipboard: clipboard not available");

        clipboard->setText(QString::fromUtf8(content));

        args.append(filePath, "COPIED_FILE");
        args.append(QString::number(content.size()), "BYTES");

        const int lines = content.count('\n') + 1;
        QString scope;
        if (lines <= 500)        scope = "<b>micro-analytic</b> - up to 500";
        else if (lines <= 1500)  scope = "<b>fine-grained</b> - up to 1500";
        else if (lines <= 3500)  scope = "<b>good</b> - upto 3500";
        else                     scope = "<b>out-of-scope</b> - over 3500";
        args.append(QString::number(lines) + " (" + scope + ")", "LINES");


        return 0;
    });

    }
//=============================================================================
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/DebugListModel.cpp ----


//=============================================================================
DebugListModel::DebugListModel()
{
}

//=============================================================================
QVariant DebugListModel::data(const QModelIndex &modelIndex, int role) const
{
    if (!modelIndex.isValid()) return QVariant();

    switch (role) {
    case item_string: return QVariant(list_.at(modelIndex.row()));
    }

    return QVariant();
}

//=============================================================================
QHash<int, QByteArray> DebugListModel::roleNames() const
{
    return
    {
    { item_string, "item_string" }
    };
}

//=============================================================================
int DebugListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return list_.count();
}

//=============================================================================
void DebugListModel::prepend(const QString& out, int max)
{
    QStringList old = list_;
    old.prepend(out);
    if ( max>0 && old.count()>max) old.removeLast();

    updateList(old);
}

//=============================================================================
void DebugListModel::setGuarded(int index, const QString& value)
{
    assert(index >= 0);
    list_[index] = value;
    QModelIndex indexModel = createIndex(index, 0);
    emit dataChanged(indexModel, indexModel);
}

//=============================================================================
void DebugListModel::updateCount(int count)
{
    if ( list_.size() < count ) {
        beginInsertRows(QModelIndex(), list_.size(), count - 1);
        while (list_.size() < count) list_.append("");
        endInsertRows();
    }
    else if ( list_.size() > count ) {
         beginRemoveRows(QModelIndex(), count, list_.size() - 1);
        while (list_.size() > count) list_.removeLast();
        endRemoveRows();
    }
}

//=============================================================================
void DebugListModel::updateItem(const QString& value, int index)
{
    if (list_.at(index) == value) return;

    list_[index] = value;
    QModelIndex i = createIndex(index, 0);
    emit dataChanged(i, i);
}

//=============================================================================
void DebugListModel::updateList(const QStringList& list)
{
    int oldSize = list_.size();
    int newSize = list.size();

    if (newSize > oldSize) {
        beginInsertRows(QModelIndex(), oldSize, newSize - 1);
        for (int i = oldSize; i < newSize; ++i) {
            list_.append(list[i]);
        }
        endInsertRows();
    }

    if (newSize < oldSize) {
        beginRemoveRows(QModelIndex(), newSize, oldSize - 1);
        while (list_.size() != newSize) list_.removeLast();
        endRemoveRows();
    }

    for (int i = 0; i < std::min(newSize, oldSize); ++i) {
        if (list[i] != list_[i]) {
            list_[i] = list[i];
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index);
        }
    }
}

//=============================================================================
QString DebugListModel::getItem(int index)
{
    if (index >= list_.count()) return "UNAVAILABLE";
    return list_[index];
}



// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/DebugListModel.h ----


class  DebugListModel : public QAbstractListModel {
    Q_OBJECT
public:
    //! @section Enums
    enum RegistryRoles {
        item_string
    };

//! @section Construction
    DebugListModel();
//! @section Items
    void updateList(const QStringList& list);
    void prepend(const QString& out, int max = -1);
    void setGuarded(int index, const QString& value);
    QString getItem(int index);
    void updateCount(int count);
    void updateItem(const QString& value, int index);
//! @section Overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &modelIndex, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
//=============================================================================
protected:
//! @section Data
    QStringList list_;
//! @section Friends
    friend class Cmds_fc_client_test;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/DebugListModelCol.h ----


class  DebugListModelCol  : public QAbstractListModel {
    Q_OBJECT
public:
//! @section Enums
    enum RegistryRoles {
        tabName = Qt::UserRole + 1,
        debugList
    };
//! @section Construction
    DebugListModelCol() = default;
    virtual ~DebugListModelCol() {}
    static DebugListModelCol& instance() { static DebugListModelCol i; return i; }
//! @section Items
    DebugListModel& get(const QString& name) {
        auto it = models_.find(name);
        if (it != models_.end()) {
            return *it.value();
        }

        auto* newModel = new DebugListModel();
        beginResetModel();
        models_.insert(name, newModel);
        modelList_.append(newModel);
        endResetModel();

        return *newModel;
    }

    QStringList     getModelNames();
//=============================================================================
//! @section overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override { return modelList_.count(); }
    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid() || index.row() < 0 || index.row() >= models_.keys().size())
            return QVariant();

        DebugListModel* model = modelList_[index.row()];

        switch (role) {
            case tabName:
                return "todo";
            case debugList:
                return QVariant::fromValue(model);
            default:
                break;
            }

        return QVariant();
    }
    QHash<int, QByteArray> roleNames() const override
    {
        return
        {
        { tabName,   "tabName" },
        { debugList, "debugList" }
        };
    }
protected:
//! @section Data
    QMap<QString, DebugListModel*> models_;
    QList<DebugListModel*>         modelList_;
//! @section Friends
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/DebugListModelItem.h ----


class  DebugListModelItem {
public:
//! @section Construction
    DebugListModelItem(const QString& header, DebugListModel* model) :
        header_(header),
        model_(model)
    {}
//! @section Items
    QString header() { return header_; }
//=============================================================================
protected:
//! @section Data
    QString         header_;
    DebugListModel* model_;
//! @section Friends
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/InteractiveOutputModel.h ----


//===================================================================
class InteractiveOutputModel {
//===================================================================
public:
//<CONSTRUCTION>
    InteractiveOutputModel() = delete;

    static DebugListModel& instance() { return DebugListModelCol::instance().get("interactiveOutput"); }

    static void append(const QString& item) {
        items_.prepend(item);
        instance().updateList(items_);
    }
//===================================================================
//<INTERNALS>
protected:
    inline static QStringList items_;
};




// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/Interactive_uiControl.h ----


#define UI_CONTROL UiControl::instance()

//===================================================================
class Interactive_uiControl : public QObject, public CmdExeGuard {
//===================================================================
    Q_OBJECT
public:
//<CONSTRUCTION>
    Interactive_uiControl(QObject *parent = nullptr) :
        QObject(parent),
        CmdExeGuard(Update)
    {
        CMD_SYS.reg(this);
    };
    static Interactive_uiControl& instance() { /*assert(!GlobalSettings::nodebug());*/
                                        static Interactive_uiControl i; return i; }
//===================================================================
//<INTERNALS>
protected:
    void commandExecuted() override {
        CmdSys& cmdsys = CMD_SYS;

        if (!myCmd_) return;
        QString args = CMD_SYS.executingArgs_.toString();
        InteractiveOutputModel::append(args);
    }

    bool myCmd_ = false;
public slots:
    void callCmd(const QString& cmd) {
        //CmdArgCol args(cmd );
        myCmd_ = true;
        CMD_SYS.execute(cmd);
        myCmd_ = false;

        //InteractiveOutput::append(args.toString());
    }
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/Model_controlsEx.h ----


class Model_controlsEx : public QAbstractListModel
//=============================================================================
{
    Q_OBJECT
public:
    enum RowType {
        TextType = 0,
        EditType,
        ButtonType
    };
    Q_ENUM(RowType)

    enum ModelRoles {
        RowTypeRole = Qt::UserRole + 1,
        ContentRole,
        CommandRole
    };

    explicit Model_controlsEx(QObject* parent = nullptr) :
        QAbstractListModel(parent)
    {
        setContent("start_stop");
    }

    virtual ~Model_controlsEx() = default;

    QString content() { return content_; }


    virtual void setContent(const QString& content) {
        content_ = content;

        beginResetModel();

        m_items.clear();

        if (content == "start_stop") {
            m_items.append({TextType, "Create table:", ""});
            m_items.append({EditType, "Edit content", ""});
            m_items.append({ButtonType, "STOP CMDS REC", "cmds_stop_record"});
            m_items.append({ButtonType, "on off message", "log_arg_message_onoff"});
        }
        else if (content == "disabled") {
            m_items.append({TextType, "Disabled", ""});
        }
        else if (content == "xxx") {
            m_items.append({TextType, "xxx", ""});
        }
        else {
            m_items.append({TextType, "no valid content", ""});
        }

        endResetModel();
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return m_items.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
            return QVariant();

        const Item &item = m_items.at(index.row());
        if (role == RowTypeRole)
            return item.rowType;
        else if (role == ContentRole)
            return item.content;
        else if (role == CommandRole)
            return item.command;
        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[RowTypeRole] = "rowType";
        roles[ContentRole] = "content";
        roles[CommandRole] = "command";
        return roles;
    }

protected:
    struct Item {
        RowType rowType;
        QString content;
        QString command;
    };

    QList<Item> m_items;
    QString     content_;
};



// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/resource/Interactive.qml ----


/// @view:include #include "ModelControls.h"

Rectangle {
    Rectangle {
        x: 10
        y: 10
        visible: true
        width: parent.width-20
        height: 30
        color: "transparent"

        Row {
            anchors.top: parent.top
            height: 40

                property string selb: "Code Focus"

                spacing: 10
                x: 0; y: 10; width: root.width-20;
                Button { text: "Code Focus";  height: parent.selb===text? 32:18;
                          onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls start_stop");}}
                Button { text: "Disable Cmd"; height: parent.selb===text? 32:18;
                          onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls disabled");}}
                Button { text: "Test Cmd";    height: parent.selb===text? 32:18;
                          onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls test");}}
            }
        }

        Rectangle {
            x: 10
            y: 40
            visible: true
            width: parent.width-20
            height: 120
            color: "#CAD7E8"

            Flow {
                width: parent.width
                height: 90
                spacing: 10

                Repeater {
                model: cmdline
                delegate: Loader {
                    id: loader
                    sourceComponent: {
                        if (rowType === 0)
                            return textComponent;
                        else if (rowType === 1)
                            return editComponent;
                        else if (rowType === 2)
                            return buttonComponent;
                    }

                    onLoaded: {
                        item.content = content;
                        if (rowType === 2) // ButtonType
                                item.command = model.command
                    }
                }
            }}

            Component {
                id: textComponent
                Text {
                    y:6
                    height: 26
                    property string content: ""
                    text: content
                }
            }

            Component {
                id: editComponent
                TextField {
                    height: 26
                    property string content: ""
                    text: content
                }
            }

            Component {
                id: buttonComponent
                Button {
                    height: 26
                    property string content: ""
                    property string command: ""
                    onClicked: { interactiveIface.callCmd(command);}
                    text: content
                }
             }
         }

    InteractiveOutput {
        color: "#EEEEFF"
        visible: true;
        x: 10
        y: 170
        height: parent.height-180;
        width: parent.width-20;
    }
}


// ---- FILE: /home/rce/KADLUB/cvz/base2/utility/resource/InteractiveOutput.qml ----

Rectangle {
    color: "#F0F0F0"
    clip: true
    ListView {
        width: parent.width; height: parent.height
        model: interactiveOutput
        spacing: 2
        delegate: Item {
            height: jobText.implicitHeight + 2;
            Rectangle {
                width: root.width-40
                height: jobText.implicitHeight + 2;
                color: "#CBD7E0"
                border.color: "darkgray"

                Text {
                    id: jobText
                    x: 10
                    width: parent.width-50
                    text: model.item_string
                    font.pixelSize: 11
                    wrapMode: Text.Wrap
                    textFormat: Text.RichText

                    onLinkActivated: { interactiveIface.callCmd(link); }
                }
            }
        }
    }
}


