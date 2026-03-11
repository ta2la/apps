#pragma once

#include "CmdSys.h"
#include "WsClientBridge.h"

#include <QEventLoop>
#include <QTimer>

//=============================================================================
class Cmds_stdio_bridge {
//=============================================================================
public:
    Cmds_stdio_bridge() = delete;
    static bool registerCmds()
    {
        CMD_SYS.add("ws_connect",
        []CMD_ARGS_U -> int {
            WsClientBridge& ws = WsClientBridge::inst();

            QEventLoop loop;
            bool success = false;

            auto connOk = QObject::connect(&ws.socket(), &QWebSocket::connected, &loop, [&]() {
                success = true;
                loop.quit();
            });
            auto connErr = QObject::connect(&ws.socket(),
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                &QWebSocket::errorOccurred,
#else
                QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
#endif
                &loop, [&]() { loop.quit(); });

            ws.openConnection();

            QTimer::singleShot(3000, &loop, &QEventLoop::quit);
            loop.exec();

            QObject::disconnect(connOk);
            QObject::disconnect(connErr);

            if (success) {
                args.append("connected", "STATUS");
                return 0;
            }
            return args.appendError("connection failed: " + ws.socket().errorString());
        });

        CMD_SYS.add("ws_disconnect",
        []CMD_ARGS_U -> int {
            WsClientBridge::inst().closeConnection();
            args.append("disconnected", "STATUS");
            return 0;
        });

        return true;
    }
};
