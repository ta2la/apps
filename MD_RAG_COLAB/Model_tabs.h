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
        items_.append(Item{ "Interactive", "qrc:/InteractiveMdRag.qml",  "\xF0\x9F\x93\x84" });
        items_.append(Item{ "Document",    "qrc:/MdDocument.qml",       "\xF0\x9F\x93\x9D" });
        items_.append(Item{ "Project",     "qrc:/MdFileManager.qml",    "\xF0\x9F\x93\x81" });
        items_.append(Item{ "Bookmarks",   "qrc:/BookmarkManager.qml",  "\xF0\x9F\x94\x96" });
        items_.append(Item{ "Cmdline",     "qrc:/TabCmdline.qml",       "\u2630" });
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
