#include "gcodeconst.h"

EngagerCommand GCodeConst::move(qreal x, qreal y, int speed) {
    return EngagerCommand(QString().sprintf("G00 X%2.4f Y%2.4f F%i\x0A", x, y, speed), 0,
                          QObject::tr("Move (%1, %2) speed=%3").arg(x).arg(y).arg(speed));
}

EngagerCommand GCodeConst::move(qreal x, qreal y, int speed, int power) {
    return EngagerCommand(QString().sprintf("G00 X%2.15f Y%2.15f L%i F%i\x0A", x, y, power, speed), 0,
                          QObject::tr("Move (%1, %2) speed=%3 power=%4").arg(x).arg(y).arg(speed).arg(power));
}

EngagerCommand GCodeConst::move(int x, int y, int speed) {
    return EngagerCommand(QString().sprintf("G00 X%i Y%i F%i\x0A", x, y, speed), 0,
                          QObject::tr("Move (%1, %2) speed=%3").arg(x).arg(y).arg(speed));
}

EngagerCommand GCodeConst::move(int x, int y, int speed, int power) {
    return EngagerCommand(QString().sprintf("G00 X%i Y%i L%i F%i\x0A", x, y, power, speed), 0,
                          QObject::tr("Move (%1, %2) speed=%3 power=%4").arg(x).arg(y).arg(speed).arg(power));
}

EngagerCommand GCodeConst::moveZero() {
    return EngagerCommand("G00 X0 Y0 F3000\x0A", 0, QObject::tr("Move to (0,0)"));
}

EngagerCommand GCodeConst::moveZero2() {
    return EngagerCommand("G00 X0 Y0 L0 F3000\x0A", 0, QObject::tr("Move to (0,0) L0"));
}

EngagerCommand GCodeConst::moveRelative(int x, int y, int speed) {
    return EngagerCommand(QString().sprintf("G01 X%i Y%i F%i\x0A", x, y, speed), 0,
                          QObject::tr("Move relative (%1,%2) speed=%3").arg(x).arg(y).arg(speed));
}

EngagerCommand GCodeConst::moveRelative(int x, int y, int speed, int power) {
    return EngagerCommand(QString().sprintf("G01 X%i Y%i L%i F%i\x0A", x, y, power, speed), 0,
                          QObject::tr("Move relative (%1, %2) speed=%3 power=%4").arg(x).arg(y).arg(speed).arg(power));
}

EngagerCommand GCodeConst::moveRelativeX(int x, int speed) {
    return EngagerCommand(QString().sprintf("G01 X%i F%i\x0A", x, speed), 0,
                          QObject::tr("Move relative X=%1 speed=%3").arg(x).arg(speed));
}

EngagerCommand GCodeConst::moveRelativeY(int y, int speed) {
    return EngagerCommand(QString().sprintf("G01 Y%i F%i\x0A", y, speed), 0,
                          QObject::tr("Move relative Y=%1 speed=%3").arg(y).arg(speed));
}

EngagerCommand GCodeConst::moveRelativeX(qreal x, int speed) {
    return EngagerCommand(QString().sprintf("G01 X%2.15f F%i\x0A", x, speed), 0,
                          QObject::tr("Move relative X=%1 speed=%3").arg(x).arg(speed));
}

EngagerCommand GCodeConst::moveRelativeY(qreal y, int speed) {
    return EngagerCommand(QString().sprintf("G01 X%2.15f F%i\x0A", y, speed), 0,
                          QObject::tr("Move relative X=%1 speed=%3").arg(y).arg(speed));
}

EngagerCommand GCodeConst::arcCW(qreal i, qreal j, int speed, int power) {
    return EngagerCommand(QString().sprintf("G02 I%2.4f J%2.4f L%i F%i\x0A", i, j, power, speed), 0,
                          QObject::tr("Arc (%1, %2) speed=%3 power=%4").arg(i).arg(j).arg(speed).arg(power));
}

EngagerCommand GCodeConst::arcCCW(qreal i, qreal j, int speed, int power) {
    return EngagerCommand(QString().sprintf("G03 I%2.4f J%2.4f L%i F%i\x0A", i, j, power, speed), 0,
                          QObject::tr("Arc (%1, %2) speed=%3 power=%4").arg(i).arg(j).arg(speed).arg(power));
}

EngagerCommand GCodeConst::delay(int ms) {
    return EngagerCommand(QString().sprintf("G04 P%i\x0A", ms), 0, QObject::tr("Pause execution for %1 ms").arg(ms));
}

EngagerCommand GCodeConst::delaySec(int sec) {
    return EngagerCommand(QString().sprintf("G04 X%i\x0A", sec), 0, QObject::tr("Pause execution for %1 s").arg(sec));
}

EngagerCommand GCodeConst::inchCoordinateSystem() {
    return EngagerCommand("G20\x0A", 0, QObject::tr("Use inch coordinate system"));
}

EngagerCommand GCodeConst::metricCoordinateSystem() {
    return EngagerCommand("G21\x0A", 0, QObject::tr("Use metric coordinate system"));
}

EngagerCommand GCodeConst::absoluteCoordinate() {
    return EngagerCommand("G90\x0A", 0, QObject::tr("Use absolute coordinate system"));
}

EngagerCommand GCodeConst::relativeCoordinate() {
    return EngagerCommand("G91\x0A", 0, QObject::tr("Use relative coordinate system"));
}

EngagerCommand GCodeConst::setNewZeroCoordinate() {
    return EngagerCommand("G91\x0A", 0, QObject::tr("Set this point as absolute (0, 0)"));
}

EngagerCommand GCodeConst::laserPower(int power1, int power2) {
    QString command;
    if (power1 > 0) {
        return EngagerCommand(QString().sprintf("M03 L%i P%i\x0A", power1, power2), 0,
                              QObject::tr("Laser power (L%1, P%2)").arg(power1).arg(power2));
    } else {
        return laserPowerOff();
    }
}

EngagerCommand GCodeConst::laserPower(int power) {
    QString command;
    if (power > 255) {
        power = 255;
    }
    double power2 = 0.00784292452830189 * static_cast<double>(power);
    if (power > 0) {
        return EngagerCommand(QString().sprintf("M03 L%i P%1.4f\x0A", power, power2), 0,
                              QObject::tr("Laser power (L%1, P%2)").arg(power).arg(power2));
    } else {
        return laserPowerOff();
    }
}

EngagerCommand GCodeConst::laserPowerOff() {
    return EngagerCommand("M05 L0\x0A", 0, QObject::tr("Laser off"));
}

EngagerCommand GCodeConst::laserPowerLow() {
    return EngagerCommand("M03 L2 P0\x0A", 0, QObject::tr("Laser low"));
}

EngagerCommand GCodeConst::laserPowerMedium() {
    return EngagerCommand("M03 L255 P0\x0A", 0, QObject::tr("Laser Middle"));
}

EngagerCommand GCodeConst::laserPowerHigh() {
    return EngagerCommand("M03 L255 P2800\x0A", 0, QObject::tr("Laser Max Power"));
}
