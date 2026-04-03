#pragma once

#include "MdItem.h"
#include <QAbstractListModel>
#include <QList>

class MdModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int cursorIndex READ cursorIndex NOTIFY cursorChanged)
public:
    enum Roles { TypeRole = Qt::UserRole + 1, TextRole, StyleRole, IsCursorRole };

    static MdModel& inst() { static MdModel i; return i; }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return items_.count();
    }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= items_.count()) return {};
        const MdItem& item = items_[index.row()];
        switch (role) {
            case TypeRole:     return item.type_;
            case TextRole:     return item.text_;
            case StyleRole:    return item.style_;
            case IsCursorRole: return index.row() == cursorIndex_;
        }
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { {TypeRole, "type"}, {TextRole, "text"}, {StyleRole, "style"}, {IsCursorRole, "isCursor"} };
    }

    int cursorIndex() const { return cursorIndex_; }

    Q_INVOKABLE void setCursorIndex(int idx) {
        if (idx < 0 || idx >= items_.count()) return;
        int old = cursorIndex_;
        cursorIndex_ = idx;
        if (old >= 0 && old < items_.count())
            emit dataChanged(index(old), index(old), {IsCursorRole});
        emit dataChanged(index(idx), index(idx), {IsCursorRole});
        emit cursorChanged();
    }

    void load(const QList<MdItem>& items) {
        beginResetModel();
        items_ = items;
        cursorIndex_ = items_.isEmpty() ? -1 : 0;
        endResetModel();
        emit cursorChanged();
    }

    const QList<MdItem>& items() const { return items_; }

signals:
    void cursorChanged();

private:
    MdModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}
    QList<MdItem> items_;
    int cursorIndex_ { -1 };
};
