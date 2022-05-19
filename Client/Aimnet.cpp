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
        _statusMessage = "AiMNET Crashed";
        emit statusMessageChanged();
    });
    connect(_aimnetProc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Aimnet::processFinished);

    // _aimnetProc->setProcessChannelMode(QProcess::ForwardedChannels);
    _aimnetProc->start(".\\InferenceEngine.exe",
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

int Aimnet::fps() const
{
    return _fps;
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
    QTextStream in(line);

    QString type;
    in >> type;

    if (type == "body")
    {
        if (_startOfBatch)
        {
            _model.clear();
            emit modelChanged();
            _startOfBatch = false;
        }

        int x, y, width, height, headX, headY;

        in >> x >> y >> width >> height >> headX >> headY;

        emit gotRect(x, y, width, height);

        _model.append(QVariantMap{
                          {"x", x},
                          {"y", y},
                          {"width", width},
                          {"height", height},
                          {"headX", headX},
                          {"headY", headY},
                      });
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
        float ms;
        in >> ms >> _fps;
        emit fpsChanged();
    }
    else if (type == "log")
    {
        _statusMessage = in.readLine();
        emit statusMessageChanged();
    }
}
