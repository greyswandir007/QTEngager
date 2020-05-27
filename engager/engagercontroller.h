#ifndef ENGAGERCONTROLLER_H
#define ENGAGERCONTROLLER_H

#include "engagercommand.h"
#include "engagerprogram.h"

#include <QObject>
#include <QTextEdit>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QProgressBar>
#include <QDateTime>
#include <QLabel>


class EngagerController : public QObject {
    Q_OBJECT
public:
    enum LaserPower {
        OFF, LOW, MIDDLE, HIGH
    };

    EngagerController();
    void sendCommand(const QString &command);
    void sendCommand(const EngagerCommand &command);
    void updateComPortList();
    QStringList getComPortList() const;
    void setTextLog(QTextEdit *textLog);
    void engagerConnect(int index);
    void engagerDisconnect();

    void runEngagerProgram(EngagerProgram *program);

    int connectedPortIndex() const;
    bool isConnected() const;
    void setAutoConnect(bool autoConnect);
    void setEngageProgressBar(QProgressBar *engageProgress);
    void setPassedTimeLabel(QLabel *passedTime);
    void setLeftTimeLabel(QLabel *leftTime);
    void clearSequence();

signals:
    void connectedToEngager();
    void disconnectedFromEngager();
    void comPortListUpdate();

private slots:
    void on_serialPortRead();
    void on_timerEvent();

private:
    QSerialPort serialPort;
    QStringList comPortList;
    QTextEdit *textLog;
    QProgressBar *engageProgress;
    QLabel *passedTimeLabel;
    QLabel *leftTimeLable;
    QTimer mainTimer;

    bool connectedFlag = false;
    bool connectedFlag2 = false;
    bool autoConnectFlag = false;
    bool connectingFlag = false;
    bool commandSend = false;
    bool zeroCoordSet = false;
    bool testMode = false;

    int connectionPortIndex = 0;
    int timePassed = 0;
    int normalTimerPeriod = 500;
    int shortTimerPeriod = 50;

    QDateTime sequenceTimeStart;

    EngagerProgram *engagerProgram = nullptr;

    void sendCommandFromSequence(const EngagerCommand &command);
    void sendNextCommand();
    void addLog(const QString &logLine);
    void clearLog();

    QString timeFromEpoch(qint64 time, bool milliseconds);
};

#endif // ENGAGERCONTROLLER_H
