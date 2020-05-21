#ifndef GCODEHELPER_H
#define GCODEHELPER_H

#include <QObject>
#include <QQueue>
#include "engagercommand.h"

typedef QQueue<EngagerCommand> CommandQueue;

class GCodeHelper {
public:
    static void reset();

    static QString createLaserPowerCommand(bool enable, int power1, int power2);
    static QString createLaserPowerCommand(int power);
    static QString createMoveCommand(qreal x, qreal y, int speed);
    static QString createMoveCommand2(qreal x, qreal y, int speed);
    static QString createArcCommand(qreal i, qreal j, int l, int speed);

    static QStringList createStartSequence();
    static QStringList createCenterSequence();
    static QStringList createMoveSequence(QString offset);
    static QStringList createMoveXSequence(qreal offset);
    static QStringList createMoveYSequence(qreal offset);
    static QStringList createMoveXSequence(int offset);
    static QStringList createMoveYSequence(int offset);
    static QStringList createMoveToSequence(qreal x, qreal y, int speed);
    static QStringList createEngageRectangleSequence();
    static QStringList createRectangleSequence(qreal x, qreal y, qreal w, qreal h, int power, int power2, int speed);
    static QStringList createCircleSequence(qreal x, qreal y, qreal r, int power, int power2, int speed);
    static QStringList createEngageImageSequence(QImage image, qreal x, qreal y, int scaleX, int scaleY, int maxIntensity);

    static CommandQueue startQueue();
    static CommandQueue centerQueue();
    static CommandQueue zeroCoordQueue();
    static CommandQueue moveXQueue(qreal offset);
    static CommandQueue moveYQueue(qreal offset);
    static CommandQueue moveXQueue(int offset);
    static CommandQueue moveYQueue(int offset);
    static CommandQueue moveToQueue(qreal x, qreal y, int speed);
    static CommandQueue engageRectangleQueue();
    static CommandQueue rectangleQueue(qreal x, qreal y, qreal w, qreal h, int power, int power2, int speed);
    static CommandQueue circleQueue(qreal x, qreal y, qreal r, int power, int power2, int speed);
    static CommandQueue engageImageQueue(QImage image, qreal x, qreal y, int scaleX, int scaleY, int maxIntensity);
};

#endif // GCODEHELPER_H
