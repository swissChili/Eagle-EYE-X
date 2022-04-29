#pragma once

#include <QObject>
#include <QProcess>
#include <qqml.h>

// TODO: Create an AimnetRectangeAdapaterModel or something
class Aimnet : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QVariantList model READ model NOTIFY modelChanged)

public:
    explicit Aimnet(QObject *parent = nullptr);
    ~Aimnet();

    QVariantList model() const;

signals:
    void batchCleared();
    void gotRect(int x, int y, int width, int height);
    void gotTime(int latency);
    void modelChanged();

private slots:
    void canRead();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

protected:
    void readLine();

private:
    QProcess *_aimnetProc;
    QVariantList _model;
};
