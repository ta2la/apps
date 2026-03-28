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

#define UI_CONTROL UiControl::inst()

class Model_tabs : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { TextRole = Qt::UserRole + 1, QmlRole, IconRole };

    explicit Model_tabs(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {
        items_.append(Item{ "Interactive", "qrc:/Interactive.qml",       "\u2699" });
        items_.append(Item{ "Cmdline",     "qrc:/TabCmdline.qml",       "\u2630" });
        items_.append(Item{ "Analyzer",    "qrc:/TabAnalyzer.qml",      "\xF0\x9F\x8C\x89" });
        items_.append(Item{ "Prompts",     "qrc:/TabPrompts.qml",       "\u00B6" });
        items_.append(Item{ "Files",       "qrc:/TabFiles.qml",         "\xF0\x9F\x97\x81" });
        items_.append(Item{ "TestModels",  "qrc:/TestModels.qml",       "\u2699" });
        items_.append(Item{ "Preview",     "qrc:/Preview.qml",          "\xF0\x9F\x91\x81" });
        items_.append(Item{ "Objects",     "qrc:/TabObjects.qml",       "\u2299" });
        items_.append(Item{ "Tree",        "qrc:/TabObjectTree.qml",    "\xF0\x9F\x8C\xB3" });
        items_.append(Item{ "Metadata",    "qrc:/TabMetadata.qml",      "\xF0\x9F\x93\x8B" });
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

///@view:end

//===================================================================

