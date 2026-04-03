#pragma once

#include <QObject>
#include <QString>

class MdItem {
    Q_GADGET
    Q_PROPERTY(int type MEMBER type_)
    Q_PROPERTY(QString text MEMBER text_)
    Q_PROPERTY(int style MEMBER style_)
public:
    enum EType { HEADING1, HEADING2, HEADING3, PARAGRAPH };
    enum EStyle { NORMAL, BOLD };

    MdItem() {}
    MdItem(EType type, const QString& text, EStyle style = NORMAL)
        : type_(type), text_(text), style_(style) {}

    int type_ { PARAGRAPH };
    QString text_;
    int style_ { NORMAL };
};

Q_DECLARE_METATYPE(MdItem)
