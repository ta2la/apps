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

    static WsClientBridge& inst(quint16 port = 12345) {
        static WsClientBridge i(port);
        return i;
    }

    void openConnection() {
        if (!signalsConnected_) {
            signalsConnected_ = true;
            connect(&socket_, &QWebSocket::textMessageReceived, this, &WsClientBridge::onMessage_);
        }
        socket_.open(QUrl(QString("ws://localhost:%1").arg(port_)));
    }

    void closeConnection() {
        socket_.close();
    }

    QWebSocket& socket() { return socket_; }

    void send(const QString& text) {
        QMetaObject::invokeMethod(this, [this, text]() {
            socket_.sendTextMessage(text);
        }, Qt::QueuedConnection);
    }

private slots:
    void onMessage_(const QString& message) {
        QTextStream ts(stdout);
        ts << message << "\n";
        ts.flush();
    }

private:
    quint16 port_;
    bool signalsConnected_ = false;
    QWebSocket socket_;
};
