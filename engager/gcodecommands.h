#ifndef GCODECOMMANDS_H
#define GCODECOMMANDS_H

#include <QObject>
#include <QRectF>
#include "engagercommand.h"
#include "igcodecommands.h"

class GCodeCommands : public IGCodeCommands {

public:
    GCodeCommands() {}
    virtual ~GCodeCommands() {}

    virtual EngagerCommand move(qreal x, qreal y, int speed); // G00
    virtual EngagerCommand move(qreal x, qreal y, int speed, int power); // G00
    virtual EngagerCommand move(int x, int y, int speed); // G00
    virtual EngagerCommand move(int x, int y, int speed, int power); // G00
    virtual EngagerCommand moveZero(); // G00
    virtual EngagerCommand moveZero2(); // G00
    virtual EngagerCommand moveRelative(int x, int y, int speed); // G01
    virtual EngagerCommand moveRelative(int x, int y, int speed, int power); // G01
    virtual EngagerCommand moveRelativeX(int x, int speed); // G01
    virtual EngagerCommand moveRelativeX(qreal x, int speed); // G01
    virtual EngagerCommand moveRelativeY(int y, int speed); // G01
    virtual EngagerCommand moveRelativeY(qreal y, int speed); // G01
    virtual EngagerCommand arcCW(qreal i, qreal j, int speed, int power); // G02
    virtual EngagerCommand arcCCW(qreal i, qreal j, int speed, int power); // G03
    virtual EngagerCommand delay(int ms); // G04
    virtual EngagerCommand delaySec(int sec); // G04

    virtual EngagerCommand inchCoordinateSystem(); // G20
    virtual EngagerCommand metricCoordinateSystem(); // G21

    virtual EngagerCommand absoluteCoordinate(); // G90
    virtual EngagerCommand relativeCoordinate(); // G91
    virtual EngagerCommand setNewZeroCoordinate(); // G92

    virtual EngagerCommand laserPower(int power1, int power2); // M03 M05
    virtual EngagerCommand laserPower(int power); // M03 M05
    virtual EngagerCommand laserPowerOff(); // M05
    virtual EngagerCommand laserPowerLow(); // M03
    virtual EngagerCommand laserPowerMedium(); // M03
    virtual EngagerCommand laserPowerHigh(); // M03

    virtual QRectF boundingRect(CommandQueue queue);

    virtual int maxSpeed();

    virtual QPixmap createPixmap(CommandQueue queue, QRectF boundingRect = QRectF());

protected:

    QString getCoord(QString cmd, QString coord);
};

#endif // GCODECOMMANDS_H
