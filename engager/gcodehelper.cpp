#include "gcodehelper.h"
#include "gcodeconst.h"
#include <QImage>

static bool zeroCoordSet = false;

void GCodeHelper::reset() {
    zeroCoordSet = false;
}

CommandQueue GCodeHelper::startQueue() {
    CommandQueue commands;
    commands.append(GCodeConst::metricCoordinateSystem());
    commands.append(GCodeConst::absoluteCoordinate());
    commands.append(GCodeConst::setNewZeroCoordinate());
    commands.append(GCodeConst::laserPowerOff());
    return commands;
}

CommandQueue GCodeHelper::centerQueue() {
    CommandQueue commands;
    commands.append(GCodeConst::laserPowerOff());
    commands.append(GCodeConst::moveZero2());
    commands.append(GCodeConst::moveZero());
    return commands;
}

CommandQueue GCodeHelper::zeroCoordQueue() {
    CommandQueue commands;
    if (!zeroCoordSet) {
        commands.append(GCodeConst::laserPowerLow());
        commands.append(GCodeConst::moveZero());
        zeroCoordSet = true;
    }
    commands.append(GCodeConst::relativeCoordinate());
    return commands;
}

CommandQueue GCodeHelper::moveXQueue(qreal offset) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(GCodeConst::moveRelativeX(offset, MAX_SPEED));
    return commands;
}

CommandQueue GCodeHelper::moveYQueue(qreal offset) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(GCodeConst::moveRelativeX(offset, MAX_SPEED));
    return commands;
}

CommandQueue GCodeHelper::moveXQueue(int offset) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(GCodeConst::moveRelativeX(offset, MAX_SPEED));
    return commands;
}

CommandQueue GCodeHelper::moveYQueue(int offset) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(GCodeConst::moveRelativeX(offset, MAX_SPEED));
    return commands;
}

CommandQueue GCodeHelper::moveToQueue(qreal x, qreal y, int speed) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(GCodeConst::move(x, y, speed));
    return commands;
}

CommandQueue GCodeHelper::engageRectangleQueue() {
    CommandQueue commands;
    commands.append(startQueue());
    commands.append(GCodeConst::move(4.404825582730206, -1.704825582730206, MAX_SPEED, 0));
    commands.append(GCodeConst::laserPowerLow());
    commands.append(GCodeConst::move(27.195174417269797, -1.704825582730206, MAX_SPEED, 0));
    commands.append(GCodeConst::move(27.195174417269797, -24.495174417269794, MAX_SPEED, 0));
    commands.append(GCodeConst::move(4.404825582730206, -24.495174417269794, MAX_SPEED, 0));
    commands.append(GCodeConst::move(4.404825582730206, -1.704825582730206, MAX_SPEED, 0));
    commands.append(GCodeConst::laserPowerOff());
    commands.append(GCodeConst::moveZero2());
    commands.append(GCodeConst::laserPowerOff());
    commands.append(GCodeConst::moveZero());
    return commands;
}

CommandQueue GCodeHelper::rectangleQueue(qreal x, qreal y, qreal w, qreal h, int power, int power2, int speed) {
    CommandQueue commands;
    commands.append(startQueue());
    commands.append(GCodeConst::move(x, y, MAX_SPEED));
    commands.append(GCodeConst::laserPower(power, power2));
    commands.append(GCodeConst::move(x+w, y, speed));
    commands.append(GCodeConst::move(x+w, y+h, speed));
    commands.append(GCodeConst::move(x, y+h, speed));
    commands.append(GCodeConst::move(x, y, speed));
    commands.append(GCodeConst::laserPowerOff());
    commands.append(centerQueue());
    return commands;
}

CommandQueue GCodeHelper::circleQueue(int speed) {
    CommandQueue commands;
    commands.append(startQueue());
    commands.append(GCodeConst::moveZero());
    commands.append(GCodeConst::setNewZeroCoordinate());
    commands.append(GCodeConst::move(5.0, -5.9, MAX_SPEED));
    commands.append(GCodeConst::arcCCW(0, 0, speed, 255));
    commands.append(GCodeConst::move(4.4048, -13.1, speed));
    commands.append(GCodeConst::arcCCW(11.3952, 0, speed, 255));
    commands.append(centerQueue());
    return commands;
}

CommandQueue GCodeHelper::circleQueue(qreal x, qreal y, qreal r, int power, int speed) {
    CommandQueue commands;
    commands.append(startQueue());
    commands.append(GCodeConst::moveZero());
    commands.append(GCodeConst::setNewZeroCoordinate());
    commands.append(GCodeConst::move(x, y, MAX_SPEED));
    commands.append(GCodeConst::arcCCW(0, 0, speed, power));
    commands.append(GCodeConst::move(x, y - r, speed));
    commands.append(GCodeConst::arcCCW(x + r, 0, speed, power));
    commands.append(centerQueue());
    return commands;
}

CommandQueue GCodeHelper::engageImageQueue(QImage image, qreal x, qreal y, qreal scaleX, qreal scaleY,
                                           int maxIntensity, bool invert) {
    CommandQueue commands;
    commands.append(startQueue());
    commands.append(GCodeConst::moveZero());
    commands.append(GCodeConst::setNewZeroCoordinate());
    qreal stepX = 0.1 * scaleX;
    qreal startX = x;
    qreal y1;
    for (int i = 0; i < image.height(); i++) {
        y1 = y;
        for (int k = 0; k < scaleY; k++) {
            x = startX;
            int j = 0;
            while (j < image.width()) {
                int power;
                if (invert) {
                    power = QColor(image.pixel(j, i)).toHsl().lightness();
                } else {
                    power = 255 - QColor(image.pixel(j, i)).toHsl().lightness();
                }
                while (j < image.width() && power == 0) {
                    x += stepX;
                    j++;
                    if (j < image.width()) {
                        if (invert) {
                            power = QColor(image.pixel(j, i)).toHsl().lightness();
                        } else {
                            power = 255 - QColor(image.pixel(j, i)).toHsl().lightness();
                        }
                    }
                }
                if (j < image.width()) {
                    commands.append(GCodeConst::move(x, y, MAX_SPEED, 0));
                    commands.append(GCodeConst::laserPower(static_cast<int>((maxIntensity/255.0) * power)));
                    if (scaleX > 1) {
                        commands.append(GCodeConst::move(x + stepX - 0.1, y, MAX_SPEED, 0));
                    }
                    commands.append(GCodeConst::laserPowerOff());
                    x += stepX;
                    j++;
                }
            }
            y += 0.1;
        }
        y = y1 + scaleY * 0.1;
    }
    commands.append(centerQueue());
    return commands;
}
