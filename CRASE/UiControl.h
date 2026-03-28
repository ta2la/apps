#pragma once

#include "CmdUiControl.h"
#include "Model_controls.h"

//=============================================================================
class UiControl : public CmdUiControl {
    Q_OBJECT
public:
    static UiControl& inst() {
        static UiControl i;
        return i;
    }
private:
    UiControl() : CmdUiControl(Model_controls::inst()) {}
};
