#ifndef ICOMMANDCREATOR_H
#define ICOMMANDCREATOR_H

#include "igcodecommands.h"


class ICommandCreator {

public:
    virtual ~ICommandCreator() {}
    virtual void reset() = 0;

    virtual CommandQueue startQueue() = 0;
    virtual CommandQueue centerQueue() = 0;
    virtual CommandQueue zeroCoordQueue() = 0;
    virtual CommandQueue moveXQueue(qreal offset) = 0;
    virtual CommandQueue moveYQueue(qreal offset) = 0;
    virtual CommandQueue moveXQueue(int offset) = 0;
    virtual CommandQueue moveYQueue(int offset) = 0;
    virtual CommandQueue moveToQueue(qreal x, qreal y, int speed) = 0;
    virtual CommandQueue engageRectangleQueue() = 0;
    virtual CommandQueue rectangleQueue(qreal x, qreal y, qreal w, qreal h, int power, int power2, int speed) = 0;
    virtual CommandQueue circleQueue(int speed) = 0;
    virtual CommandQueue circleQueue(qreal x, qreal y, qreal r, int power, int speed) = 0;
    virtual CommandQueue engageImageQueue(QImage image, qreal x, qreal y, qreal scale, int maxIntensity,
                                          bool invert = false, bool mirrorX = false, bool mirrorY = false) = 0;
    virtual QRectF imageRect(QImage image, qreal x, qreal y, qreal scale) = 0;

    virtual IGCodeCommands *gcodeCommands() = 0;
};

#endif // ICOMMANDCREATOR_H
