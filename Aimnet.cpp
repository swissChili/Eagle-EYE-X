#include "Aimnet.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>

Aimnet::Aimnet(QObject *parent)
    : QObject{parent}
    , _aimnetProc{new QProcess(this)}
{
    connect(_aimnetProc, &QProcess::readyReadStandardOutput, this, &Aimnet::canRead);
    connect(_aimnetProc, &QProcess::started, this, [&]()
    {
        qDebug() << "process started";
    });
    connect(_aimnetProc, &QProcess::errorOccurred, this, [&](auto err)
    {
        qDebug() << "Error" << err;
    });
    connect(_aimnetProc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Aimnet::processFinished);

    _aimnetProc->setProcessChannelMode(QProcess::ForwardedChannels);
    _aimnetProc->start("powershell.exe",
                       QStringList() << QDir::currentPath() + "/AiMNET/start.ps1"
                       , QIODevice::ReadOnly);
}

Aimnet::~Aimnet()
{
    _aimnetProc->terminate();
}

QVariantList Aimnet::model() const
{
    return _model;
}

void Aimnet::canRead()
{
    if (_aimnetProc->canReadLine())
    {
        qDebug() << "Can read";
        readLine();
    }
}

void Aimnet::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << exitCode << exitStatus;
    QString outerr = QString::fromUtf8(_aimnetProc->readAllStandardError());
    qDebug() << "stderr" << outerr;
}

// TODO: BUG:
// For some reason this function is SUPER delayed. Like it takes ~10-15 seconds to get through
// the 4 lines of JSON that AiMNET sends. The ~200 ms timings that get sent from python *are* accurate,
// it's something on the C++ end because if you move around, the results get "unsynced" and show
// old positions of players.
// If this data was being queued somewhere, it would appear in a process's memory, but both C++
// and Python executables are stable at around ~200MB each, which seems correct. Maybe that means
// the data is getting lost in the pipe?
// Next step to debug: forward all stdout and see if it's delayed too. Yes, it appears delayed, but
// not by that much. Next idea: use a websocket.
void Aimnet::readLine()
{
    QElapsedTimer timer;
    timer.start();

    QByteArray line = _aimnetProc->readLine();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(line, &err);

    if (err.error != QJsonParseError::NoError)
        return;

    QJsonObject object = doc.object();
    QString type = object["type"].toString();

    qDebug() << "Got line" << QString::fromLatin1(line);

    if (type == "body")
    {
        emit gotRect(object["x"].toInt(),
                object["y"].toInt(),
                object["width"].toInt(),
                object["height"].toInt());

        _model.append(object.toVariantMap());
        emit modelChanged();
    }
    else if (type == "batch_done")
    {
        emit batchCleared();
        _model.clear();
        emit modelChanged();
    }
    else if (type == "time")
    {
        emit gotTime(object["ms"].toInt());
    }

    qDebug() << "Processing line took" << timer.elapsed();
}
