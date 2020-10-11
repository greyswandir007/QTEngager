#include "gcodecommands.h"

#include <QPainter>
#include <QPixmap>

EngagerCommand GCodeCommands::move(qreal x, qreal y, int speed) {
    return EngagerCommand(QString().sprintf("G00 X%2.4f Y%2.4f F%i\x0A", x, y, speed), 0,
                          QObject::tr("Move (%1, %2) speed=%3").arg(x).arg(y).arg(speed));
}

EngagerCommand GCodeCommands::move(qreal x, qreal y, int speed, int power) {
    return EngagerCommand(QString().sprintf("G00 X%2.15f Y%2.15f L%i F%i\x0A", x, y, power, speed), 0,
                          QObject::tr("Move (%1, %2) speed=%3 power=%4").arg(x).arg(y).arg(speed).arg(power));
}

EngagerCommand GCodeCommands::move(int x, int y, int speed) {
    return EngagerCommand(QString().sprintf("G00 X%i Y%i F%i\x0A", x, y, speed), 0,
                          QObject::tr("Move (%1, %2) speed=%3").arg(x).arg(y).arg(speed));
}

EngagerCommand GCodeCommands::move(int x, int y, int speed, int power) {
    return EngagerCommand(QString().sprintf("G00 X%i Y%i L%i F%i\x0A", x, y, power, speed), 0,
                          QObject::tr("Move (%1, %2) speed=%3 power=%4").arg(x).arg(y).arg(speed).arg(power));
}

EngagerCommand GCodeCommands::moveZero() {
    return EngagerCommand("G00 X0 Y0 F3000\x0A", 0, QObject::tr("Move to (0,0)"));
}

EngagerCommand GCodeCommands::moveZero2() {
    return EngagerCommand("G00 X0 Y0 L0 F3000\x0A", 0, QObject::tr("Move to (0,0) L0"));
}

EngagerCommand GCodeCommands::moveRelative(int x, int y, int speed) {
    return EngagerCommand(QString().sprintf("G01 X%i Y%i F%i\x0A", x, y, speed), 0,
                          QObject::tr("Move relative (%1,%2) speed=%3").arg(x).arg(y).arg(speed));
}

EngagerCommand GCodeCommands::moveRelative(int x, int y, int speed, int power) {
    return EngagerCommand(QString().sprintf("G01 X%i Y%i L%i F%i\x0A", x, y, power, speed), 0,
                          QObject::tr("Move relative (%1, %2) speed=%3 power=%4").arg(x).arg(y).arg(speed).arg(power));
}

EngagerCommand GCodeCommands::moveRelativeX(int x, int speed) {
    return EngagerCommand(QString().sprintf("G01 X%i F%i\x0A", x, speed), 0,
                          QObject::tr("Move relative X=%1 speed=%3").arg(x).arg(speed));
}

EngagerCommand GCodeCommands::moveRelativeX(qreal x, int speed) {
    return EngagerCommand(QString().sprintf("G01 X%2.15f F%i\x0A", x, speed), 0,
                          QObject::tr("Move relative X=%1 speed=%3").arg(x).arg(speed));
}

EngagerCommand GCodeCommands::moveRelativeY(int y, int speed) {
    return EngagerCommand(QString().sprintf("G01 Y%i F%i\x0A", y, speed), 0,
                          QObject::tr("Move relative Y=%1 speed=%3").arg(y).arg(speed));
}

EngagerCommand GCodeCommands::moveRelativeY(qreal y, int speed) {
    return EngagerCommand(QString().sprintf("G01 Y%2.15f F%i\x0A", y, speed), 0,
                          QObject::tr("Move relative Y=%1 speed=%3").arg(y).arg(speed));
}

EngagerCommand GCodeCommands::arcCW(qreal i, qreal j, int speed, int power) {
    return EngagerCommand(QString().sprintf("G02 I%2.4f J%2.4f L%i F%i\x0A", i, j, power, speed), 0,
                          QObject::tr("Arc (%1, %2) speed=%3 power=%4").arg(i).arg(j).arg(speed).arg(power));
}

EngagerCommand GCodeCommands::arcCCW(qreal i, qreal j, int speed, int power) {
    return EngagerCommand(QString().sprintf("G03 I%2.4f J%2.4f L%i F%i\x0A", i, j, power, speed), 0,
                          QObject::tr("Arc (%1, %2) speed=%3 power=%4").arg(i).arg(j).arg(speed).arg(power));
}

EngagerCommand GCodeCommands::delay(int ms) {
    return EngagerCommand(QString().sprintf("G04 P%i\x0A", ms), 0, QObject::tr("Pause execution for %1 ms").arg(ms));
}

EngagerCommand GCodeCommands::delaySec(int sec) {
    return EngagerCommand(QString().sprintf("G04 X%i\x0A", sec), 0, QObject::tr("Pause execution for %1 s").arg(sec));
}

EngagerCommand GCodeCommands::inchCoordinateSystem() {
    return EngagerCommand("G20\x0A", 0, QObject::tr("Use inch coordinate system"));
}

EngagerCommand GCodeCommands::metricCoordinateSystem() {
    return EngagerCommand("G21\x0A", 0, QObject::tr("Use metric coordinate system"));
}

EngagerCommand GCodeCommands::absoluteCoordinate() {
    return EngagerCommand("G90\x0A", 0, QObject::tr("Use absolute coordinate system"));
}

EngagerCommand GCodeCommands::relativeCoordinate() {
    return EngagerCommand("G91\x0A", 0, QObject::tr("Use relative coordinate system"));
}

EngagerCommand GCodeCommands::setNewZeroCoordinate() {
    return EngagerCommand("G92\x0A", 0, QObject::tr("Set this point as absolute (0, 0)"));
}

EngagerCommand GCodeCommands::laserPower(int power1, int power2) {
    if (power1 > 0) {
        return EngagerCommand(QString().sprintf("M03 L%i P%i\x0A", power1, power2), 0,
                              QObject::tr("Laser power (L%1, P%2)").arg(power1).arg(power2));
    }
    return laserPowerOff();
}

EngagerCommand GCodeCommands::laserPower(int power) {
    QString command;
    if (power > 255) {
        power = 255;
    }
    double power2 = 0.00784292452830189 * static_cast<double>(power);
    if (power > 0) {
        return EngagerCommand(QString().sprintf("M03 L%i P%1.4f\x0A", power, power2), 0,
                              QObject::tr("Laser power (L%1, P%2)").arg(power).arg(power2));
    }
    return laserPowerOff();
}

EngagerCommand GCodeCommands::laserPowerOff() {
    return EngagerCommand("M05 L0\x0A", 0, QObject::tr("Laser off"));
}

EngagerCommand GCodeCommands::laserPowerLow() {
    return EngagerCommand("M03 L2 P0\x0A", 0, QObject::tr("Laser low"));
}

EngagerCommand GCodeCommands::laserPowerMedium() {
    return EngagerCommand("M03 L255 P0\x0A", 0, QObject::tr("Laser Middle"));
}

EngagerCommand GCodeCommands::laserPowerHigh() {
    return EngagerCommand("M03 L255 P2800\x0A", 0, QObject::tr("Laser Max Power"));
}

QRectF GCodeCommands::boundingRect(CommandQueue queue) {
    bool inited = false;
    QRectF rect;
    qreal minX = 0;
    qreal minY = 0;
    qreal maxX = 0;
    qreal maxY = 0;
    qreal lastX = 0;
    qreal lastY = 0;
    for (EngagerCommand command : queue) {
        QString cmd = command.getCommand();
        if (cmd.startsWith("G00")) {
            QString Xstr = getCoord(cmd, "X");
            QString Ystr = getCoord(cmd, "Y");
            lastX = Xstr.toDouble();
            lastY = Ystr.toDouble();
        } else if (cmd.startsWith("G01")) {
            QString Xstr = getCoord(cmd, "X");
            QString Ystr = getCoord(cmd, "Y");
            if (!Xstr.isEmpty()) {
                lastX += Xstr.toDouble();
            }
            if (!Ystr.isEmpty()) {
                lastY += Ystr.toDouble();
            }
        } else if (cmd.startsWith("G02")) {
            QString Xstr = getCoord(cmd, "I");
            QString Ystr = getCoord(cmd, "J");
            if (!Xstr.isEmpty()) {
                lastX = Xstr.toDouble();
            }
            if (!Ystr.isEmpty()) {
                lastY = Ystr.toDouble();
            }
        } else if (cmd.startsWith("G03")) {
            QString Xstr = getCoord(cmd, "I");
            QString Ystr = getCoord(cmd, "J");
            if (!Xstr.isEmpty()) {
                lastX = Xstr.toDouble();
            }
            if (!Ystr.isEmpty()) {
                lastY = Ystr.toDouble();
            }
        }
        if (inited) {
            if (lastX > maxX) {
                maxX = lastX;
            } else if (lastX < minX) {
                minX = lastX;
            }
            if (lastY > maxY) {
                maxY = lastY;
            } else if (lastY < minY) {
                minY = lastY;
            }
        } else {
            maxX = lastX;
            minX = lastX;
            maxY = lastY;
            minY = lastY;
            inited = true;
        }
    }
    if (inited) {
        rect.setX(minX);
        rect.setY(minY);
        rect.setWidth(maxX - minX);
        rect.setHeight(maxY - minY);
    }
    return rect;
}

int GCodeCommands::maxSpeed() {
    return 3000;
}

QPixmap GCodeCommands::createPixmap(CommandQueue queue, QRectF boundingRect) {
    if (boundingRect == QRectF()) {
        boundingRect = this->boundingRect(queue);
    }
    QPixmap pixmap(static_cast<int>(boundingRect.width() * 10) + 1, static_cast<int>(boundingRect.height() * 10) + 1);
    pixmap.fill(QColor::fromRgb(255, 255, 255));
    int intensity = 0;
    qreal lastX = 0;
    qreal lastY = 0;
    QPainter painter(&pixmap);
    for (EngagerCommand command : queue) {
        QString cmd = command.getCommand();
        qreal x = lastX;
        qreal y = lastY;
        if (cmd.startsWith("G00")) {
            QString Xstr = getCoord(cmd, "X");
            QString Ystr = getCoord(cmd, "Y");
            x = Xstr.toDouble() * 10;
            y = Ystr.toDouble() * 10;
        } else if (cmd.startsWith("G01")) {
            QString Xstr = getCoord(cmd, "X");
            QString Ystr = getCoord(cmd, "Y");
            if (!Xstr.isEmpty()) {
                x += Xstr.toDouble() * 10;
            }
            if (!Ystr.isEmpty()) {
                y += Ystr.toDouble() * 10;
            }
        } else if (cmd.startsWith("G02")) {
            QString Xstr = getCoord(cmd, "I");
            QString Ystr = getCoord(cmd, "J");
            if (!Xstr.isEmpty()) {
                x = Xstr.toDouble() * 10;
            }
            if (!Ystr.isEmpty()) {
                y = Ystr.toDouble() * 10;
            }
        } else if (cmd.startsWith("G03")) {
            QString Xstr = getCoord(cmd, "I");
            QString Ystr = getCoord(cmd, "J");
            if (!Xstr.isEmpty()) {
                x = Xstr.toDouble() * 10;
            }
            if (!Ystr.isEmpty()) {
                y = Ystr.toDouble() * 10;
            }
        } else if (cmd.startsWith("M05")) {
            intensity = 0;
        } else if (cmd.startsWith("M03")) {
            QString Lstr = getCoord(cmd, "L");
            if (!Lstr.isEmpty()) {
                intensity = Lstr.toInt();
            }
        }
        if (intensity != 0) {
            painter.setPen(QColor(255 - intensity, 255 - intensity, 255 - intensity));
            painter.drawLine(static_cast<int>(lastX), static_cast<int>(lastY), static_cast<int>(x),
                             static_cast<int>(y));
        }
        lastX = x;
        lastY = y;
    }
    painter.end();
    return pixmap;
}

QString GCodeCommands::getCoord(QString cmd, QString coord) {
    if (cmd.contains(coord)) {
        QString str = cmd.mid(cmd.indexOf(coord) + 1, cmd.length());
        return str.left(str.indexOf(str.contains(' ') ? ' ' : '\x0A'));
    }
    return QString();
}
