#pragma once

#include <QAbstractListModel>
#include <QList>

//=============================================================================
class Model_tabs : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { TextRole = Qt::UserRole + 1, QmlRole, IconRole };

    explicit Model_tabs(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {
        items_.append(Item{ "Interactive", "qrc:/Interactive.qml",       "\xF0\x9F\x8C\x80" });
        items_.append(Item{ "Cmdline",     "qrc:/TabCmdline.qml",       "\u2630" });
        items_.append(Item{ "Objects",     "qrc:/TabObjects.qml",       "\u2299" });
        items_.append(Item{ "Tree",        "qrc:/TabObjectTree.qml",    "\xF0\x9F\x8C\xB3" });
        items_.append(Item{ "Drawing",     "qrc:/TabDrawing.qml",       "\xF0\x9F\x93\x90" });
        items_.append(Item{ "Metadata",    "qrc:/TabMetadata.qml",      "\xF0\x9F\x93\x8B" });
        items_.append(Item{ "Claude",      "qrc:/TabClaude.qml",        "\xE2\x9C\xB4\xEF\xB8\x8F" });
        items_.append(Item{ "About",       "qrc:/About.qml",            "\u24D8" });
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent)
        return items_.count();
    }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid()) return {};
        const Item& it = items_.at(index.row());
        switch (role) {
        case TextRole: return it.text;
        case QmlRole:  return it.qml;
        case IconRole: return it.icon;
        }
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { { TextRole, "text" }, { QmlRole, "qml" }, { IconRole, "icon" } };
    }

private:
    struct Item { QString text; QString qml; QString icon; };
    QList<Item> items_;
};
