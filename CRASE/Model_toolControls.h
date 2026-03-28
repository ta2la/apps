#pragma once

#include "Model_controlsEx.h"

///@view:beg

class Model_controlsSettings : public Model_controlsEx
{
    Q_OBJECT
public:
    static Model_controlsSettings& inst() { static Model_controlsSettings i; return i; }

    void setContent(const QString& content) override
    {
        content_ = content;
        beginResetModel();
        m_items.clear();

        m_items.append({TextType, "Model_controlsSettings content", ""});

        endResetModel();
    }

private:
    Model_controlsSettings() : Model_controlsEx() { setContent("default"); }
};

///@view:end
