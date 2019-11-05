#ifndef ENGAGERCONTROLLER_H
#define ENGAGERCONTROLLER_H

#include <QObject>
#include <QTextEdit>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QProgressBar>
#include <QDateTime>
#include <QLabel>

#define MAX_SPEED   3000

class EngagerController : public QObject {
    Q_OBJECT
public:
    enum LaserPower {
        OFF, LOW, MIDDLE, HIGH
    };

    EngagerController();
    void sendCommand(QString command);
    void addCommand(QString command);
    void addCommandList(QStringList commands);
    void updateComPortList();
    QStringList getComPortList();
    void setTextLog(QTextEdit *textLog);
    void engagerConnect(int index);
    void engagerDisconnect();
    void createLaserPowerCommand(LaserPower power);
    void moveX(qreal offset);
    void moveY(qreal offset);
    void moveX(int offset);
    void moveY(int offset);
    void moveTo(qreal x, qreal y, int speed);
    void rectangle();
    void rectangle(qreal x, qreal y, qreal w, qreal h, int power, int power2, int speed);
    void circle();
    void circle(qreal x, qreal y, qreal r, int power, int power2, int speed);
    int connectedPortIndex();
    bool isConnected();
    void setAutoConnect(bool autoConnect);
    void setEngageProgressBar(QProgressBar *engageProgress);
    void setPassedTimeLabel(QLabel *passedTime);
    void setLeftTimeLabel(QLabel *leftTime);
    void clearSequence();
    void engageImage(QImage image, qreal x, qreal y, int scaleX, int scaleY, int maxIntensity);

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
    QStringList commandList;
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
    int totalCommandCount;
    int passedCommandCount;

    QDateTime sequenceTimeStart;

    void sendCommandFromSequence(QString command);
    void sendNextCommand();
    void move(QString offset);
    void addLog(QString logLine);

    QString createLaserPowerCommand(bool enable, int power1, int power2);
    QString createLaserPowerCommand(int power);
    QString createMoveCommand(qreal x, qreal y, int speed);
    QString createMoveCommand2(qreal x, qreal y, int speed);
    QString createArcCommand(qreal i, qreal j, int l, int speed);
    QStringList getStartSequence();
    QStringList getCenterSequence();
};

#endif // ENGAGERCONTROLLER_H
