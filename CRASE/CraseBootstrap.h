#pragma once

#include "CmdSys.h"
#include "CraseEmFs.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>

//=============================================================================
// QML façade for the CRASE WASM bootstrap modal.
// Triggers CraseEmFs operations + polls until completion + emits signals.
// Hardcoded list of remote DB URLs for the user to choose from.
//=============================================================================
class CraseBootstrap : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString     dirName    READ dirName    NOTIFY changed)
    Q_PROPERTY(QString     status     READ status     NOTIFY changed)
    Q_PROPERTY(QStringList localFiles READ localFiles NOTIFY changed)
    Q_PROPERTY(QStringList remoteUrls READ remoteUrls CONSTANT)
    Q_PROPERTY(QStringList remoteNames READ remoteNames CONSTANT)
public:
    static CraseBootstrap& inst() { static CraseBootstrap b; return b; }

    QString dirName() const  { return CraseEmFs::pickedDirName(); }
    QString status()  const  { return status_; }
    QStringList localFiles() const { return localFiles_; }
    QStringList remoteUrls() const {
        return { "https://t2ls.com/t2lcad/examples/tanks.db" };
    }
    QStringList remoteNames() const {
        return { "tanks.db (t2ls.com)" };
    }

    // Step 1: pick directory.
    Q_INVOKABLE void pickDir() {
        status_ = "picking"; emit changed();
        CraseEmFs::pickDir();
        startPoll([this](int s) {
            if (s == 1) {
                // refresh local file list
                CraseEmFs::listDir();
                startPoll([this](int s2) {
                    if (s2 == 1) {
                        localFiles_ = CraseEmFs::listedFiles();
                        QStringList dbs;
                        for (const QString& f : localFiles_)
                            if (f.endsWith(".db", Qt::CaseInsensitive)) dbs << f;
                        localFiles_ = dbs;
                        status_ = "picked"; emit changed();
                    } else {
                        status_ = "list-error"; emit changed();
                    }
                }, []{ return CraseEmFs::listDirStatus(); });
            } else {
                status_ = "pick-error"; emit changed();
            }
        }, []{ return CraseEmFs::pickDirStatus(); });
    }

    // Step 2a: open existing local DB file.
    Q_INVOKABLE void openLocal(const QString& fileName) {
        status_ = "loading"; emit changed();
        CraseEmFs::copyLocalToMemfs(fileName);
        startPoll([this, fileName](int s) {
            if (s == 1) connectSqlite(fileName);
            else { status_ = "load-error"; emit changed(); }
        }, []{ return CraseEmFs::copyLocalStatus(); });
    }

    // Step 2b: download remote URL → dir + MEMFS, then open.
    Q_INVOKABLE void openRemote(int index) {
        QStringList urls = remoteUrls();
        QStringList names = remoteNames();
        if (index < 0 || index >= urls.size()) return;
        QString url = urls[index];
        // derive filename (last path segment, strip query)
        QString fileName = url.section('/', -1).section('?', 0, 0);
        if (fileName.isEmpty()) fileName = "downloaded.db";
        status_ = "downloading"; emit changed();
        CraseEmFs::downloadUrlBinary(url, fileName);
        startPoll([this](int s) {
            if (s == 1) connectSqlite(CraseEmFs::lastDownloadName());
            else { status_ = "download-error"; emit changed(); }
        }, []{ return CraseEmFs::downloadStatus(); });
    }

signals:
    void changed();

private:
    CraseBootstrap(QObject* p = nullptr) : QObject(p) {}

    template <typename Cb, typename Poll>
    void startPoll(Cb cb, Poll poll) {
        auto* t = new QTimer(this);
        t->setInterval(200);
        QObject::connect(t, &QTimer::timeout, [t, cb, poll]() {
            int s = poll();
            if (s == 0 || s == -2) return;
            t->stop(); t->deleteLater();
            cb(s);
        });
        t->start();
    }

    void connectSqlite(const QString& fileName) {
        QString cmd = QString("db_connect --name tanks_sqlite --driver sqlite --path /%1 --active")
                          .arg(fileName);
        CMD_SYS.execute_threadSafe(cmd);
        status_ = "connected"; emit changed();
    }

    QString     status_ = "idle";
    QStringList localFiles_;
};
