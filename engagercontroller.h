#ifndef ENGAGERCONTROLLER_H
#define ENGAGERCONTROLLER_H

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
    void sendCommand(QString command);
    void updateComPortList();
    QStringList getComPortList();
    void setTextLog(QTextEdit *textLog);
    void engagerConnect(int index);
    void engagerDisconnect();

    void runEngagerProgram(EngagerProgram *program);

    int connectedPortIndex();
    bool isConnected();
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

    bool connectedFlag;
    bool connectedFlag2;
    bool autoConnectFlag;
    bool connectingFlag;
    bool commandSend;
    bool zeroCoordSet;

    int connectionPortIndex;
    int timePassed;

    QDateTime sequenceTimeStart;

    EngagerProgram *engagerProgram;

    void sendCommandFromSequence(QString command);
    void sendNextCommand();
    void addLog(QString logLine);
    void clearLog();
};

#endif // ENGAGERCONTROLLER_H
