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

#include "Model_controls.h"

#include <CmdSys.h>
#include <CmdExeGuard.h>

#include <QObject>

///@view:beg

#define UI_CONTROL UiControl::inst()

//===================================================================
class UiControl : public QObject, public CmdExeGuard {
//===================================================================
    Q_OBJECT
public:
//<CONSTRUCTION>
    UiControl(QObject *parent = nullptr) :
        QObject(parent),
        CmdExeGuard(Update)
    {
        CMD_SYS.reg(this);
    };
    static UiControl& inst() { static UiControl i; return i; }
//! Methods
    void     setRootObject(QObject* object) { rootObject_ = object; }
    QObject* rootObject()                   { return rootObject_; }
//===================================================================
//<INTERNALS>
//<OVERRIDES>
    void commandExecuted() override {
        Model_controls::inst().setContent(Model_controls::inst().content());
        if (rootObject_ == nullptr) return;
        QMetaObject::invokeMethod(rootObject_, "update", Qt::QueuedConnection);
    }
protected:
//<DATA>
    QObject *rootObject_ {nullptr};
//!Qt

public slots:
    void callCmd(const QString& cmd) {
        CMD_SYS.execute(cmd);
    }
};

///@view:end

//===================================================================

