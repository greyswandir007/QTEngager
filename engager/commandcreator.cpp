#include "commandcreator.h"
#include <QImage>

CommandCreator::CommandCreator(IGCodeCommands *gcodeCommands) {
    this->gcode = gcodeCommands;
}

void CommandCreator::reset() {
    zeroCoordSet = false;
}

CommandQueue CommandCreator::startQueue() {
    CommandQueue commands;
    commands.append(gcode->metricCoordinateSystem());
    commands.append(gcode->absoluteCoordinate());
    commands.append(gcode->setNewZeroCoordinate());
    commands.append(gcode->laserPowerOff());
    return commands;
}

CommandQueue CommandCreator::centerQueue() {
    CommandQueue commands;
    commands.append(gcode->laserPowerOff());
    commands.append(gcode->moveZero2());
    commands.append(gcode->moveZero());
    return commands;
}

CommandQueue CommandCreator::zeroCoordQueue() {
    CommandQueue commands;
    if (!zeroCoordSet) {
        commands.append(gcode->laserPowerLow());
        commands.append(gcode->moveZero());
        zeroCoordSet = true;
    }
    commands.append(gcode->relativeCoordinate());
    return commands;
}

CommandQueue CommandCreator::moveXQueue(int offset) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(gcode->moveRelativeX(offset, gcode->maxSpeed()));
    return commands;
}

CommandQueue CommandCreator::moveXQueue(qreal offset) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(gcode->moveRelativeX(offset, gcode->maxSpeed()));
    return commands;
}

CommandQueue CommandCreator::moveYQueue(int offset) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(gcode->moveRelativeY(offset, gcode->maxSpeed()));
    return commands;
}

CommandQueue CommandCreator::moveYQueue(qreal offset) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(gcode->moveRelativeY(offset, gcode->maxSpeed()));
    return commands;
}

CommandQueue CommandCreator::moveToQueue(qreal x, qreal y, int speed) {
    CommandQueue commands = zeroCoordQueue();
    commands.append(gcode->move(x, y, speed));
    return commands;
}

CommandQueue CommandCreator::engageRectangleQueue() {
    CommandQueue commands;
    commands.append(startQueue());
    commands.append(gcode->move(4.404825582730206, -1.704825582730206, gcode->maxSpeed(), 0));
    commands.append(gcode->laserPowerLow());
    commands.append(gcode->move(27.195174417269797, -1.704825582730206, gcode->maxSpeed(), 0));
    commands.append(gcode->move(27.195174417269797, -24.495174417269794, gcode->maxSpeed(), 0));
    commands.append(gcode->move(4.404825582730206, -24.495174417269794, gcode->maxSpeed(), 0));
    commands.append(gcode->move(4.404825582730206, -1.704825582730206, gcode->maxSpeed(), 0));
    commands.append(gcode->laserPowerOff());
    commands.append(gcode->moveZero2());
    commands.append(gcode->laserPowerOff());
    commands.append(gcode->moveZero());
    return commands;
}

CommandQueue CommandCreator::rectangleQueue(qreal x, qreal y, qreal w, qreal h, int power, int power2, int speed) {
    CommandQueue commands;
    commands.append(startQueue());
    commands.append(gcode->move(x, y, gcode->maxSpeed()));
    commands.append(gcode->laserPower(power, power2));
    commands.append(gcode->move(x + w, y, speed));
    commands.append(gcode->move(x + w, y + h, speed));
    commands.append(gcode->move(x, y + h, speed));
    commands.append(gcode->move(x, y, speed));
    commands.append(gcode->laserPowerOff());
    commands.append(centerQueue());
    return commands;
}

CommandQueue CommandCreator::circleQueue(int speed) {
    CommandQueue commands;
    commands.append(startQueue());
    commands.append(gcode->moveZero());
    commands.append(gcode->setNewZeroCoordinate());
    commands.append(gcode->move(5.0, -5.9, gcode->maxSpeed()));
    commands.append(gcode->arcCCW(0, 0, speed, 255));
    commands.append(gcode->move(4.4048, -13.1, speed));
    commands.append(gcode->arcCCW(11.3952, 0, speed, 255));
    commands.append(centerQueue());
    return commands;
}

CommandQueue CommandCreator::circleQueue(qreal x, qreal y, qreal r, int power, int speed) {
    CommandQueue commands;
    commands.append(startQueue());
    commands.append(gcode->moveZero());
    commands.append(gcode->setNewZeroCoordinate());
    commands.append(gcode->move(x, y, gcode->maxSpeed()));
    commands.append(gcode->arcCCW(0, 0, speed, power));
    commands.append(gcode->move(x, y - r, speed));
    commands.append(gcode->arcCCW(x + r, 0, speed, power));
    commands.append(centerQueue());
    return commands;
}

CommandQueue CommandCreator::engageImageQueue(QImage image, qreal x, qreal y, qreal scale, int maxIntensity,
                                              bool invert, bool mirrorX, bool mirrorY, int numberOfPasses) {
    CommandQueue baseCommands;
    baseCommands.append(startQueue());
    baseCommands.append(gcode->moveZero());
    baseCommands.append(gcode->setNewZeroCoordinate());
    qreal stepX = 0.1 * scale;
    qreal startX = x;
    int sign = invert ? 1 : -1;
    int add = invert ? 0 : 255;
    bool heightComplete = false;
    int dirX = mirrorX ? -1 : 1;
    int dirY = mirrorY ? -1 : 1;
    int i = mirrorY ? image.height() - 1 : 0;
    CommandQueue commands;
    while (!heightComplete) {
        qreal y1 = y;
        for (int k = 0; k < scale; k++) {
            x = startX;
            bool widthComplete = false;
            int j = mirrorX ? image.width() - 1 : 0;
            while (!widthComplete) {
                int power = add + sign * lightness(image, j, i);
                while (!widthComplete && power == 0) {
                    x += stepX;
                    j += dirX;
                    if (j < 0 || j >= image.width()) {
                        widthComplete = true;
                    } else {
                        power = add + sign * lightness(image, j, i);
                    }
                }
                if (!widthComplete) {
                    commands.append(gcode->move(x, -y, gcode->maxSpeed(), 0));
                    commands.append(gcode->laserPower(static_cast<int>((maxIntensity / 255.0) * power)));
                    if (scale > 1) {
                        commands.append(gcode->move(x + stepX - 0.1, -y, gcode->maxSpeed(), 0));
                    }
                    commands.append(gcode->laserPowerOff());
                    x += stepX;
                    j += dirX;
                    if (j < 0 || j >= image.width()) {
                        widthComplete = true;
                    }
                }
            }
            y += 0.1;
        }
        y = y1 + scale * 0.1;
        i += dirY;
        if (i < 0 || i >= image.height()) {
            heightComplete = true;
        }
    }
    for (int i = 0; i < numberOfPasses; i++) {
        baseCommands.append(commands);
    }
    baseCommands.append(centerQueue());
    return baseCommands;
}

QRectF CommandCreator::imageRect(QImage image, qreal x, qreal y, qreal scale) {
    QRectF rect;
    qreal height = scale * 0.1 * image.height();
    rect.setLeft(x);
    rect.setTop(y - height);
    rect.setWidth(scale * 0.1 * image.width());
    rect.setHeight(height);
    return rect;
}

IGCodeCommands *CommandCreator::gcodeCommands() {
    return gcode;
}

int CommandCreator::lightness(QImage image, int x, int y) {
    return QColor(image.pixel(x, y)).toHsl().lightness();
}
