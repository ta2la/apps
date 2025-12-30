The following commits completed using **PROMPT ASSEMBLER**.

[COMMIT 1](https://github.com/ta2la/apps/commit/7b9357b85b5ec71b5e5606dabe927831529e51ab)<br>
[COMMIT 2](https://github.com/ta2la/base2/commit/cf8720011216b6ca3af5baab0596c4b190bcc6f5)<br>

Follows all used prompts, answers simplified for readability.

An interesting aspect is the introduction, where the concept is clarified and simplified. Only afterward does the code generation follow. At the end, there is debugging of errors.

<hr>

**PROMPT:**<br>
PROMPT ASSEMBLY<br>
\+<br>
tabAnalyzer is in common lib cmd_sys_display 
i need to shift in code_analyser and add in GenericApp 
dynamically 
suggestions

**REPLY:**

...

Create a small, neutral model that represents tabs:

    class UiTabRegistry : public QAbstractListModel {
    Q_OBJECT
    public:
    enum Roles {
        ObjectNameRole = Qt::UserRole + 1,
        TitleRole,
        QmlSourceRole
    };

    static UiTabRegistry& instance();

    void registerTab(
        const QString& objectName,
        const QString& title,
        const QUrl& qmlSource
    );

    // QAbstractListModel overrides
};

...

Replace the hard-coded tabAnalyzer block with:

    Repeater {
    model: uiTabRegistry

    delegate: Loader {
        id: tabLoader
        objectName: objectName
        source: qmlSource
        visible: false
        anchors.fill: parent
    }}

<hr>    

 **PROMPT:**<br>   
we need model for buttons only and load single content, not model

**REPLY:**

...

<hr>

**PROMPT:**<br>
he buttons i understand<br> 
create new tab for now just for tests tabLoadTest<br>
two buttons there and load different qml content there

**REPLY:**

...

    Rectangle {
        color: "#F0F0F0"

        Column {
            anchors.fill: parent
            spacing: 10
            padding: 10

            Row {
                spacing: 10

                Button {
                    text: "Load A"
                    onClicked: contentLoader.source = "LoadTestA.qml"
                }

                Button {
                    text: "Load B"
                    onClicked: contentLoader.source = "LoadTestB.qml"
                }
            }

            Rectangle {
                width: parent.width
                height: parent.height - 50
                color: "#DADADA"

                Loader {
                    id: contentLoader
                    anchors.fill: parent
                }
            }
        }
    }

...

<hr>

**PROMPT:**<br>
onClicked: contentLoader.source = "LoadTestB.qml"<br><br>
this is part of model in future<br> 
LoadTestB.qml can be in any static lib i guess

**REPLY:**

Correct — and this is an important detail to get right early.

...

<hr>

**PROMPT:**<br>
Qt.resolvedUrl("qrc:/loadtest/LoadTestB.qml")<br>
not necessary so complex<br>
LoadTestB.qml will be unique for sure

**REPLY:**

...

<hr>

**PROMPT:**<br>
loadtest - why this if in each static lib is qml in root

**REPLY:**

...

<hr>

**PROMPT:**<br>
PROMPT ASSEMBLY<br>
\+<br>
so giving the context again<br>
just organise your thoughts to solve the task with<br> 
GenericApp.qml<br> 
analyzerModule is also needed<br> 
do not code for now<br>

**REPLY:**

...

<hr>

**PROMPT:**<br> 
so rearange tabAnalyzer in own qml now

    Rectangle {
        objectName: "tabAnalyzer"
        id: tabAnalyzer

        visible: false
        x: 10; y: 50
        width: root.width - 20
        height: root.height - 60
        color: "#F0F0F0"

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 4

            Repeater {
                model: analyzerModules

                delegate: Text {
                    text: moduleData.module
                    font.pointSize: 12
                }
            }
        }
    }

**REPLY:**    

...

<hr>

**PROMPT:**<br> 
tak mrkni, zatim TabAnalyzer.qml aplikace staticka takze ted butony stale staticke, Interactive a TabAnalyzer jako loader

**REPLY:**

...

<hr>

**PROMPT:**<br> 
shrnuti pekne, tak ted snad kod

**REPLY:**

GenericApp.qml

    Rectangle {
        id: root
        anchors.fill: parent
        color: "#D9D9D9"

        Rectangle {
            id: mainSpace
            x: 0
            y: 0
            width: root.width
            height: 40
            color: "#4cc4bc"

            Image {
                x: 2
                y: 2
                width: 32
                height: 32
                source: "images/icon.svg"
            }

            Text {
                x: 40
                y: 8
                font.pointSize: 14
                font.bold: true
                text: "PROMPT Assembler"
            }

            Button {
                x: 260 + 120
                y: 10
                width: 110
                height: 30
                text: "Cmdline"
                onClicked: contentLoader.source = "qrc:/Interactive.qml"
            }

            Button {
                x: 260 + 120 * 3
                y: 10
                width: 110
                height: 30
                text: "Analyzer"
                onClicked: contentLoader.source = "qrc:/TabAnalyzer.qml"
            }
        }

        Loader {
            id: contentLoader
            x: 10
            y: 50
            width: root.width - 20
            height: root.height - 60

            // výchozí pohled
            source: "qrc:/Interactive.qml"
        }
    }

TabAnalyzer.qml    

Rectangle {
    color: "#F0F0F0"
    anchors.fill: parent

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 4

        Repeater {
            model: analyzerModules

            delegate: Text {
                text: moduleData.module
                font.pointSize: 12
            }
        }
    }
}

<hr>

**PROMPT:**

    Rectangle { id: mainSpace; color: "#4cc4bc" x: 0; y: 0; width: root.width; height: 40 Image {x:2; y:2; width:32; height:32; source:"images/icon.svg"} Text {x:40; y: 8; font.pointSize: 14; font.bold: true; text: "PROMPT Assembler" } Button { id: buttonInteractive; x:260+120; y:10; width: 110; height: 30; text: "Cmdline"; onClicked: { activateTab("tabInteractive"); contentLoader.source = "qrc:/Interactive.qml; } } Button { id: buttonAnalyzer x: 260 + 120*3 y: 10 width: 110 height: 30 text: "Analyzer" onClicked: { activateTab("tabAnalyzer"); contentLoader.source = "qrc:/TabAnalyzer.qml"; } } } 

najdi chyby?

**REPLY:**

Tady máš neuzavřený string:

contentLoader.source = "qrc:/Interactive.qml;

...

<hr>

**PROMPT:**<br>
PROMPT ASSEMBLY<br>
\+<br>
je videt Cmdline - ale neprepne na Analyzer????

**REPLY:**

V GenericApp.qml máš stále:

    function activateTab(tabName) {
        tabInteractive.visible = (tabInteractive.objectName === tabName) ? true : false;
        tabAnalyzer.visible    = (tabAnalyzer.objectName === tabName)
    }

....

<hr>

**PROMPT:**<br>
PROMPT ASSEMBLY<br>
\+<br>
ted v GenericApp.qml vyrobime reakci na model pro butony<br>
model c++, bude v PROMPT_ASSEMBLER

**REPLY:**

...

    class Model_mainTabs : public QAbstractListModel {
        Q_OBJECT
    public:
        enum Roles {
            TextRole = Qt::UserRole + 1,
            QmlRole
        };

        explicit Model_mainTabs(QObject* parent = nullptr)
            : QAbstractListModel(parent)
        {
            // staticky pro začátek
            items_.append({ "Cmdline",  "qrc:/Interactive.qml" });
            items_.append({ "Analyzer", "qrc:/TabAnalyzer.qml" });
        }

        int rowCount(const QModelIndex& parent = QModelIndex()) const override {
            Q_UNUSED(parent)
            return items_.count();
        }

        QVariant data(const QModelIndex& index, int role) const override {
            if (!index.isValid())
                return {};

            const Item& it = items_.at(index.row());

            switch (role) {
                case TextRole: return it.text;
                case QmlRole:  return it.qml;
            }
            return {};
        }

        QHash<int, QByteArray> roleNames() const override {
            return {
                { TextRole, "text" },
                { QmlRole,  "qml"  }
            };
        }

    private:
        struct Item {
            QString text;
            QString qml;
        };

        QList<Item> items_;
    };

...

    engine.rootContext()->setContextProperty(
        "mainTabs",
        new Model_mainTabs(&engine)
    );    

...

    Row {
        x: 260
        y: 10
        spacing: 10

        Repeater {
            model: mainTabs

            delegate: Button {
                width: 110
                height: 30
                text: model.text

                onClicked: {
                    contentLoader.source = model.qml
                }
            }
        }
    }

<hr>    

**PROMPT:**<br> 

12:12:26: Debugging /home/rce/KADLUB/cvz/APPS/PROMPT_ASSEMBLER/debug/PROMPT_ASSEMBLER -qmljsdebugger=port:46123,block,services:DebugMessages,QmlDebugger,V8Debugger,QmlInspector,DebugTranslation ... QML debugging is enabled. Only use this in a safe environment. qt.svg: Cannot open file '/home/rce/KADLUB/cvz/APPS/PROMPT_ASSEMBLER/debug/qrc:/images/icon.svg', because: No such file or directory qt.svg: Cannot open file '/home/rce/KADLUB/cvz/APPS/PROMPT_ASSEMBLER/debug/qrc:/images/icon.svg', because: No such file or directory QML Debugger: Waiting for connection on port 46123... qrc:/Interactive.qml:49: ReferenceError: cmdline is not defined qrc:/InteractiveOutput.qml:12: ReferenceError: interactiveOutput is not defined qrc:/InteractiveOutput.qml:12: ReferenceError: interactiveOutput is not defined qrc:/Interactive.qml:49: ReferenceError: cmdline is not defined qrc:/InteractiveOutput.qml:12: ReferenceError: interactiveOutput is not defined qrc:/Interactive.qml:49: ReferenceError: cmdline is not defined qrc:/InteractiveOutput.qml:12: ReferenceError: interactiveOutput is not defined qrc:/InteractiveOutput.qml:12: ReferenceError: interactiveOutput is not defined qt.qml.context: qrc:/InteractiveOutput.qml:31:21 Parameter "link" is not declared. Injection of parameters into signal handlers is deprecated. Use JavaScript functions with formal parameters instead. 13:59:05: Debugging of /home/rce/KADLUB/cvz/APPS/PROMPT_ASSEMBLER/debug/PROMPT_ASSEMBLER -qmljsdebugger=port:46123,block,services:DebugMessages,QmlDebugger,V8Debugger,QmlInspector,DebugTranslation has finished with exit code 0.

**REPLY:**

...

<hr>



