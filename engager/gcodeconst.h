#ifndef GCODECONST_H
#define GCODECONST_H

#include "engagercommand.h"

#define MAX_SPEED   3000

class GCodeConst {
public:
    enum LaserPower {
        OFF, LOW, MIDDLE, HIGH
    };
    static EngagerCommand move(qreal x, qreal y, int speed); // G00
    static EngagerCommand move(qreal x, qreal y, int speed, int power); // G00
    static EngagerCommand move(int x, int y, int speed); // G00
    static EngagerCommand move(int x, int y, int speed, int power); // G00
    static EngagerCommand moveZero(); // G00
    static EngagerCommand moveZero2(); // G00
    static EngagerCommand moveRelative(int x, int y, int speed); // G01
    static EngagerCommand moveRelative(int x, int y, int speed, int power); // G01
    static EngagerCommand moveRelativeX(int x, int speed); // G01
    static EngagerCommand moveRelativeY(int y, int speed); // G01
    static EngagerCommand moveRelativeX(qreal x, int speed); // G01
    static EngagerCommand moveRelativeY(qreal y, int speed); // G01
    static EngagerCommand arcCW(qreal i, qreal j, int speed, int power); // G02
    static EngagerCommand arcCCW(qreal i, qreal j, int speed, int power); // G03
    static EngagerCommand delay(int ms); // G04
    static EngagerCommand delaySec(int sec); // G04

    static EngagerCommand inchCoordinateSystem(); // G20
    static EngagerCommand metricCoordinateSystem(); // G21

    static EngagerCommand absoluteCoordinate(); // G90
    static EngagerCommand relativeCoordinate(); // G91
    static EngagerCommand setNewZeroCoordinate(); // G92

    static EngagerCommand laserPower(int power1, int power2); // M03 M05
    static EngagerCommand laserPower(int power); // M03 M05
    static EngagerCommand laserPowerOff(); // M05
    static EngagerCommand laserPowerLow(); // M03
    static EngagerCommand laserPowerMedium(); // M03
    static EngagerCommand laserPowerHigh(); // M03
};

#endif // GCODECONST_H
