#pragma once

#include <QObject>
#include <QProcess>
#include <qqml.h>
#include <QTimer>

// TODO: Create an AimnetRectangeAdapaterModel or something
class Aimnet : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QVariantList model READ model NOTIFY modelChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit Aimnet(QObject *parent = nullptr);
    ~Aimnet();

    QVariantList model() const;
    QString statusMessage() const;

signals:
    void batchCleared();
    void gotRect(int x, int y, int width, int height);
    void gotTime(int latency);
    void modelChanged();
    void statusMessageChanged();

private slots:
    void canRead();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

protected:
    void readLine();

private:
    QProcess *_aimnetProc;
    QVariantList _model;
    QTimer _readTimer;
    bool _startOfBatch = false;
    QString _statusMessage = "";
};
