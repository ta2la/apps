#pragma once

#include "CmdSys.h"

#include <QObject>
#include <QWebSocket>

//=============================================================================
class WsClientBridge : public QObject {
    Q_OBJECT
public:
    WsClientBridge(quint16 port = 12345)
        : QObject(nullptr)
        , port_(port)
    {
    }

    static WsClientBridge& init(quint16 port = 12345) {
        static WsClientBridge i(port);
        i.connect_();
        return i;
    }

private:
    void connect_() {
        if (connected_) return;
        connected_ = true;
        connect(&socket_, &QWebSocket::connected, this, &WsClientBridge::onConnected_);
        connect(&socket_, &QWebSocket::textMessageReceived, this, &WsClientBridge::onMessage_);
        connect(&socket_, &QWebSocket::disconnected, this, &WsClientBridge::onDisconnected_);
        socket_.open(QUrl(QString("ws://localhost:%1").arg(port_)));
    }

private slots:
    void onConnected_() {
    }

    void onMessage_(const QString& message) {
        QTextStream ts(stdout);
        ts << message << "\n";
        ts.flush();
    }

    void onDisconnected_() {
    }

public:
    void send(const QString& text) {
        QMetaObject::invokeMethod(this, [this, text]() {
            socket_.sendTextMessage(text);
        }, Qt::QueuedConnection);
    }

private:
    quint16 port_;
    bool connected_ = false;
    QWebSocket socket_;
};
