//
// Copyright (C) 2025 Petr Talla. [petr.talla@gmail.com]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//		      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=============================================================================
/**
 * @class Cmds_test
 * @brief Basic test of CmdSys.
 */
#pragma once

#include <CmdSys.h>
#include <CmdExeGuard.h>

#include <QAbstractListModel>
#include <QList>

///@view:beg

#define UI_CONTROL UiControl::instance()

class Model_tabs : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        TextRole = Qt::UserRole + 1,
        QmlRole
    };

    explicit Model_tabs(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {
        // staticky pro začátek
        items_.append(Item{ "Interactive", "qrc:/Interactive.qml" });
        items_.append(Item{ "Cmdline",     "qrc:/TabCmdline.qml" });
        items_.append(Item{ "Analyzer",    "qrc:/TabAnalyzer.qml" });
        items_.append(Item{ "About",       "qrc:/About.qml" });
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

///@view:end

//===================================================================

