#ifndef GCODEHELPER_H
#define GCODEHELPER_H

#include <QObject>
#include <QQueue>
#include "engagercommand.h"

typedef QQueue<EngagerCommand> CommandQueue;

class GCodeHelper {
public:
    static void reset();

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
    static CommandQueue circleQueue(int speed);
    static CommandQueue circleQueue(qreal x, qreal y, qreal r, int power, int speed);
    static CommandQueue engageImageQueue(QImage image, qreal x, qreal y, qreal scale, int maxIntensity,
                                         bool invert = false, bool mirrorX = false, bool mirrorY = false);

private:
    static int lightness(QImage image, int x, int y);
};

#endif // GCODEHELPER_H
