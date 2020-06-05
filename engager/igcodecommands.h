#ifndef IGCODECOMMANDS_H
#define IGCODECOMMANDS_H

#include <QObject>
#include <QQueue>
#include "engagercommand.h"

typedef QQueue<EngagerCommand> CommandQueue;

class IGCodeCommands {

public:
    virtual ~IGCodeCommands() {}

    virtual EngagerCommand move(qreal x, qreal y, int speed) = 0; // G00
    virtual EngagerCommand move(qreal x, qreal y, int speed, int power) = 0; // G00
    virtual EngagerCommand move(int x, int y, int speed) = 0; // G00
    virtual EngagerCommand move(int x, int y, int speed, int power) = 0; // G00
    virtual EngagerCommand moveZero() = 0; // G00
    virtual EngagerCommand moveZero2() = 0; // G00
    virtual EngagerCommand moveRelative(int x, int y, int speed) = 0; // G01
    virtual EngagerCommand moveRelative(int x, int y, int speed, int power) = 0; // G01
    virtual EngagerCommand moveRelativeX(int x, int speed) = 0; // G01
    virtual EngagerCommand moveRelativeX(qreal x, int speed) = 0; // G01
    virtual EngagerCommand moveRelativeY(int y, int speed) = 0; // G01
    virtual EngagerCommand moveRelativeY(qreal y, int speed) = 0; // G01
    virtual EngagerCommand arcCW(qreal i, qreal j, int speed, int power) = 0; // G02
    virtual EngagerCommand arcCCW(qreal i, qreal j, int speed, int power) = 0; // G03
    virtual EngagerCommand delay(int ms) = 0; // G04
    virtual EngagerCommand delaySec(int sec) = 0; // G04

    virtual EngagerCommand inchCoordinateSystem() = 0; // G20
    virtual EngagerCommand metricCoordinateSystem() = 0; // G21

    virtual EngagerCommand absoluteCoordinate() = 0; // G90
    virtual EngagerCommand relativeCoordinate() = 0; // G91
    virtual EngagerCommand setNewZeroCoordinate() = 0; // G92

    virtual EngagerCommand laserPower(int power1, int power2) = 0; // M03 M05
    virtual EngagerCommand laserPower(int power) = 0; // M03 M05
    virtual EngagerCommand laserPowerOff() = 0; // M05
    virtual EngagerCommand laserPowerLow() = 0; // M03
    virtual EngagerCommand laserPowerMedium() = 0; // M03
    virtual EngagerCommand laserPowerHigh() = 0; // M03

    virtual QRectF boundingRect(CommandQueue queue) = 0;

    virtual int maxSpeed() = 0;
};

#endif // IGCODECOMMANDS_H
