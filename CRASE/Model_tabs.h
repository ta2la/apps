#pragma once

#include <QAbstractListModel>
#include <QList>

//=============================================================================
class Model_tabs : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { TextRole = Qt::UserRole + 1, QmlRole };

    explicit Model_tabs(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {
        items_.append(Item{ "Interactive", "qrc:/Interactive.qml" });
        items_.append(Item{ "Cmdline",     "qrc:/TabCmdline.qml" });
        items_.append(Item{ "Objects",     "qrc:/TabObjects.qml" });
        items_.append(Item{ "Tree",        "qrc:/TabObjectTree.qml" });
        items_.append(Item{ "Metadata",    "qrc:/TabMetadata.qml" });
        items_.append(Item{ "About",       "qrc:/About.qml" });
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
        }
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { { TextRole, "text" }, { QmlRole, "qml" } };
    }

private:
    struct Item { QString text; QString qml; };
    QList<Item> items_;
};
