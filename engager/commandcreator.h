#ifndef COMMANDCREATOR_H
#define COMMANDCREATOR_H

#include "icommandcreator.h"

class CommandCreator : public ICommandCreator {

public:
    CommandCreator(IGCodeCommands *gcode);
    virtual ~CommandCreator() {}

    virtual void reset();

    virtual CommandQueue startQueue();
    virtual CommandQueue centerQueue();
    virtual CommandQueue zeroCoordQueue();
    virtual CommandQueue moveXQueue(int offset);
    virtual CommandQueue moveXQueue(qreal offset);
    virtual CommandQueue moveYQueue(int offset);
    virtual CommandQueue moveYQueue(qreal offset);
    virtual CommandQueue moveToQueue(qreal x, qreal y, int speed);
    virtual CommandQueue engageRectangleQueue();
    virtual CommandQueue rectangleQueue(qreal x, qreal y, qreal w, qreal h, int power, int power2, int speed);
    virtual CommandQueue circleQueue(int speed);
    virtual CommandQueue circleQueue(qreal x, qreal y, qreal r, int power, int speed);
    virtual CommandQueue engageImageQueue(QImage image, qreal x, qreal y, qreal scale, int maxIntensity,
                                          bool invert = false, bool mirrorX = false, bool mirrorY = false);

    virtual IGCodeCommands *gcodeCommands();
private:
    bool zeroCoordSet = false;
    IGCodeCommands *gcode;

    int lightness(QImage image, int x, int y);
};

#endif // COMMANDCREATOR_H
