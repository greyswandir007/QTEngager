#include "gcodehelper.h"
#include <QImage>

static bool zeroCoordSet = false;

void GCodeHelper::reset() {
    zeroCoordSet = false;
}

QString GCodeHelper::createLaserPowerCommand(GCodeHelper::LaserPower power) {
    switch (power) {
    case LaserPower::OFF:
        return "M05 L0\x0A";
    case LaserPower::LOW:
        return "M3 L2 P0\x0A";
    case LaserPower::MIDDLE:
        return "M3 L255 P0\x0A";
    case LaserPower::HIGH:
        return "M3 L255 P2800\x0A";
    }
    return "";
}

QString GCodeHelper::createLaserPowerCommand(bool enable, int power1, int power2) {
    QString command;
    if (enable) {
        return QString().sprintf("M3 L%i P%i\x0A", power1, power2);
    } else {
        return "M05 L0\x0A";
    }
}

QString GCodeHelper::createLaserPowerCommand(int power) {
    QString command;
    if (power > 255) {
        power = 255;
    }
    double power2 = 0.00784292452830189 * static_cast<double>(power);
    if (power > 0) {
        return QString().sprintf("M3 L%i P%1.4f\x0A", power, power2);
    } else {
        return "M05 L0\x0A";
    }
}

QString GCodeHelper::createMoveCommand(qreal x, qreal y, int speed) {
    return QString().sprintf("G00 X%2.15f Y%2.15f L0 F%i\x0A", x, y, speed);
}

QString GCodeHelper::createMoveCommand2(qreal x, qreal y, int speed) {
    return QString().sprintf("G00 X%2.4f Y%2.4f F%i\x0A", x, y, speed);
}

QString GCodeHelper::createArcCommand(qreal i, qreal j, int l, int speed) {
    return QString().sprintf("G03 I%2.4f J%2.4f L%i F%i\x0A", i, j, l, speed);
}

QStringList GCodeHelper::createStartSequence() {
    QStringList commands;
    commands.append("G21\x0A");
    commands.append("G90\x0A");
    commands.append("G92\x0A");
    commands.append("M05 L0\x0A");
    return commands;
}

QStringList GCodeHelper::createCenterSequence() {
    QStringList commands;
    commands.append("M05 L0\x0A");
    commands.append("G00 X0 Y0 L0 F3000\x0A");
    commands.append("G00 X0 Y0 F3000\x0A");
    return commands;
}

QStringList GCodeHelper::createMoveSequence(QString offset) {
    QStringList commands;
    if (!zeroCoordSet) {
        commands.append("M05 L0\x0A");
        commands.append("G00 X0 Y0 F3000\x0A");
        zeroCoordSet = true;
    }
    commands.append("G91\x0A");
    commands.append("G01 " + offset + " L0 F3000\x0A");
    return commands;
}

QStringList GCodeHelper::createMoveXSequence(qreal offset) {
    return createMoveSequence(QString().sprintf("X%2.15f", offset));
}

QStringList GCodeHelper::createMoveYSequence(qreal offset) {
    return createMoveSequence(QString().sprintf("Y%2.15f", offset));
}

QStringList GCodeHelper::createMoveXSequence(int offset) {
    return createMoveSequence(QString().sprintf("X%i", offset));
}

QStringList GCodeHelper::createMoveYSequence(int offset) {
    return createMoveSequence(QString().sprintf("Y%i", offset));
}

QStringList GCodeHelper::createMoveToSequence(qreal x, qreal y, int speed) {
    QStringList commands;
    if (!zeroCoordSet) {
        commands.append("M05 L0\x0A");
        commands.append("G00 X0 Y0 F3000\x0A");
        zeroCoordSet = true;
    }
    commands.append("G91\x0A");
    commands.append(createMoveCommand(x, y, speed));
    return commands;
}

QStringList GCodeHelper::createEngageRectangleSequence() {
    QStringList commands;
    commands.append("G21\x0A");
    commands.append("G92\x0A");
    commands.append("G90\x0A");
    commands.append("M05 P0 L0\x0A");
    commands.append("G00 X4.404825582730206 Y-1.704825582730206 L0 F3000\x0A");
    commands.append("M03 L2 P0\x0A");
    commands.append("G00 X27.195174417269797 Y-1.704825582730206 L0 F3000\x0A");
    commands.append("G00 X27.195174417269797 Y-24.495174417269794 L0 F3000\x0A");
    commands.append("G00 X4.404825582730206 Y-24.495174417269794 L0 F3000\x0A");
    commands.append("G00 X4.404825582730206 Y-1.704825582730206 L0 F3000\x0A");
    commands.append("M05 L0\x0A");
    commands.append("G00 X0 Y0 L0 F3000\x0A");
    commands.append("M05 L0\x0A");
    commands.append("G00 X0 Y0 F3000\x0A");

    return commands;
}

QStringList GCodeHelper::createRectangleSequence(qreal x, qreal y, qreal w, qreal h, int power, int power2, int speed) {
    QStringList commands;
    commands.append(createStartSequence());
    commands.append(createMoveCommand(x, y, MAX_SPEED));
    commands.append(createLaserPowerCommand(true, power, power2));
    commands.append(createMoveCommand(x+w, y, speed));
    commands.append(createMoveCommand(x+w, y+h, speed));
    commands.append(createMoveCommand(x, y+h, speed));
    commands.append(createMoveCommand(x, y, speed));
    commands.append(createLaserPowerCommand(false, 0, 0));
    commands.append(createCenterSequence());

    return commands;
}

QStringList GCodeHelper::createCircleSequence(qreal x, qreal y, qreal r, int power, int power2, int speed) {
    QStringList commands;
    commands.append(createStartSequence());
    commands.append("G00 X0 Y0 F3000\x0A");
    commands.append("G92\x0A");
    commands.append(createMoveCommand2(5, -5.9, 3000));
    commands.append(createArcCommand(0, 0, 255, speed));
    commands.append(createMoveCommand2(4.4048, -13.1, speed));
    commands.append(createArcCommand(11.3952, 0, 255, speed));
    commands.append(createCenterSequence());

    return commands;
}

QStringList GCodeHelper::createEngageImageSequence(QImage image, qreal x, qreal y, int scaleX, int scaleY, int maxIntensity) {
    QStringList commands;
    commands.append(createStartSequence());
    commands.append("G00 X0 Y0 F3000\x0A");
    commands.append("G92\x0A");
    qreal stepX = 0.1 * scaleX;
    qreal startX = x;
    for (int i = 0; i < image.height(); i++) {
        for (int k = 0; k < scaleY; k++) {
            x = startX;
            for (int j = 0; j < image.width(); j++) {
                int power = 255 - QColor(image.pixel(j, i)).toHsl().lightness();
                commands.append(createMoveCommand(x, y, MAX_SPEED));
                commands.append(createLaserPowerCommand(static_cast<int>((maxIntensity/255.0) * power)));
                if (scaleX > 1) {
                    commands.append(createMoveCommand(x + stepX - 0.1, y, MAX_SPEED));
                }
                commands.append("M05 L0\x0A");
                x += stepX;
            }
            y += 0.1;
        }
    }
    commands.append(createCenterSequence());
    return commands;
}
