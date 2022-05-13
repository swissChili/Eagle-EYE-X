#include "Aimnet.h"

#include <IPCMessage.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>

Aimnet::Aimnet(QObject *parent)
    : QObject{parent}
    , _aimnetProc{new QProcess(this)}
{
    _readTimer.setInterval(50);
    connect(&_readTimer, &QTimer::timeout, this, &Aimnet::canRead);

    connect(_aimnetProc, &QProcess::readyReadStandardOutput, this, &Aimnet::canRead);
    connect(_aimnetProc, &QProcess::started, this, [&]()
    {
        qDebug() << "process started";
    });
    connect(_aimnetProc, &QProcess::errorOccurred, this, [&](auto err)
    {
        qDebug() << "Error" << err;
        _statusMessage = "AiMNET Crashed: " + err;
        emit statusMessageChanged();
    });
    connect(_aimnetProc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Aimnet::processFinished);

    // _aimnetProc->setProcessChannelMode(QProcess::ForwardedChannels);
    _aimnetProc->setWorkingDirectory(".\\AiMNET-Server\\x64\\Debug\\InferenceEngine");
    _aimnetProc->start(".\\AiMNET-Server\\x64\\Debug\\InferenceEngine\\InferenceEngine.exe",
                       QStringList(),
                       QIODevice::ReadOnly);
}

Aimnet::~Aimnet()
{
    _aimnetProc->terminate();
    _aimnetProc->kill();
}

QVariantList Aimnet::model() const
{
    return _model;
}

QString Aimnet::statusMessage() const
{
    return _statusMessage;
}

void Aimnet::canRead()
{
    while (_aimnetProc->bytesAvailable() >= IPC::messageSize)
    {
        readMessage();
    }
}

void Aimnet::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << exitCode << exitStatus;
    QString outerr = QString::fromUtf8(_aimnetProc->readAllStandardError());
    qDebug() << "stderr" << outerr;
}

void Aimnet::readMessage()
{
    if (_aimnetProc->bytesAvailable() < IPC::messageSize)
    {
        return;
    }

    QByteArray messageBuffer = _aimnetProc->read(IPC::messageSize);

    if (messageBuffer.size() < IPC::messageSize)
    {
        qDebug() << "Reading over pipe failed -- not enough bytes";
        qFatal("Not enough bytes");
    }

    const IPC::Message &msg = *((IPC::Message *)messageBuffer.data());

    if (msg.type == IPC::MessageType::BODY)
    {
        if (_startOfBatch)
        {
            _model.clear();
            emit modelChanged();
            _startOfBatch = false;
        }

        _model.append(QVariantMap{
                          {"x", msg.body.x},
                          {"y", msg.body.y},
                          {"width", msg.body.width},
                          {"height", msg.body.height},
                          {"headX", msg.body.headX},
                          {"headY", msg.body.headY},
                      });

        emit modelChanged();
    }
    else if (msg.type == IPC::MessageType::BATCH_DONE)
    {
        if (_startOfBatch)
        {
            _model.clear();
            emit modelChanged();
        }

        emit batchCleared();
        _startOfBatch = true;
    }
    else if (msg.type == IPC::MessageType::LOG)
    {
        QString log(msg.log.message);
        _statusMessage = log;
        emit statusMessageChanged();
    }
}
