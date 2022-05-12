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
    while (_aimnetProc->canReadLine())
    {
        readLine();
    }
}

void Aimnet::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << exitCode << exitStatus;
    QString outerr = QString::fromUtf8(_aimnetProc->readAllStandardError());
    qDebug() << "stderr" << outerr;
}

void Aimnet::readLine()
{
    QByteArray line = _aimnetProc->readLine();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(line, &err);

    if (err.error != QJsonParseError::NoError)
        return;

    QJsonObject object = doc.object();
    QString type = object["type"].toString();

    if (type == "body")
    {
        if (_startOfBatch)
        {
            _model.clear();
            emit modelChanged();
            _startOfBatch = false;
        }

        emit gotRect(object["x"].toInt(),
                object["y"].toInt(),
                object["width"].toInt(),
                object["height"].toInt());

        _model.append(object.toVariantMap());
        emit modelChanged();
    }
    else if (type == "batch_done")
    {
        if (_startOfBatch)
        {
            _model.clear();
            emit modelChanged();
        }

        emit batchCleared();
        _startOfBatch = true;
    }
    else if (type == "time")
    {
        emit gotTime(object["ms"].toInt());
    }
    else if (type == "log")
    {
        _statusMessage = object["message"].toString();
        emit statusMessageChanged();
    }
}
