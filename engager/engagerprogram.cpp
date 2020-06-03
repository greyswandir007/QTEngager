#include "engagerprogram.h"

#include <QFile>
#include <QGraphicsItem>
#include <QDebug>

#include "components/graphicitems/maingraphicsitem.h"
#include "components/graphicitems/mainsvgitem.h"



EngagerProgram::EngagerProgram() {
    totalCount = 0;
    passedCount = 0;
}

EngagerProgram::EngagerProgram(const QString &filename) {
    loadProgram(filename);
}

EngagerProgram::EngagerProgram(const CommandQueue &engagerProgram) {
    this->engagerProgram = engagerProgram;
    totalCount = engagerProgram.count();
    passedCount = 0;
}

EngagerProgram::EngagerProgram(MainView *mainView) {
    commandQueueFromMainView(mainView);
}

CommandQueue EngagerProgram::currentProgram() const {
    return engagerProgram;
}

void EngagerProgram::setCurrentProgram(const CommandQueue &program) {
    engagerProgram = program;
    totalCount = engagerProgram.count();
    passedCount = 0;
}

void EngagerProgram::loadProgram(const QString &filename) {
    QFile file(filename);
    engagerProgram.clear();
    if (file.open(QIODevice::ReadOnly)) {
        QString str = str.fromLatin1(file.readAll());
        QStringList program = str.split("\x0A");
        for (int i = 0; i < program.size(); i++) {
            if (program [i].endsWith("\x0D")) {
                program [i].truncate(engagerProgram[i].getCommand().length()-1);
            }
            program[i] += "\x0A";
            engagerProgram.append(EngagerCommand(program[i]));
        }
        file.close();
        totalCount = engagerProgram.count();
        passedCount = 0;
    }
}

void EngagerProgram::saveProgram(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QString program = "";
        for(EngagerCommand command : engagerProgram) {
            program += command.getCommand();
        }
        file.write(program.toLatin1());
        file.close();
    }
}

void EngagerProgram::newProgram() {
    engagerProgram.clear();
    totalCount = 0;
    passedCount = 0;
}

void EngagerProgram::addCommand(const CommandQueue &command) {
    engagerProgram.append(command);
    totalCount++;
}

EngagerCommand EngagerProgram::pullCommand() {
    EngagerCommand command;
    if (!engagerProgram.isEmpty()) {
        command = engagerProgram.at(0);
        engagerProgram.removeAt(0);
        passedCount++;
    }
    return command;
}

bool EngagerProgram::hasCommand() const {
    return !engagerProgram.isEmpty();
}

int EngagerProgram::totalCommandCount() const {
    return totalCount;
}

int EngagerProgram::passedCommandCount() const {
    return passedCount;
}

int EngagerProgram::leftCommandCount() const {
    return engagerProgram.count();
}

float EngagerProgram::getCurrentProgress() const {
    return engagerProgram.isEmpty() ? 0 : passedCount * 100.0f / static_cast<float>(totalCount);
}

void EngagerProgram::commandQueueFromMainView(MainView *mainView) {
    engagerProgram.clear();
    for(QGraphicsItem *item : mainView->scene()->items()) {
        qreal multiply = 0.1 / item->data(MAIN_SCALE_FACTOR).toDouble();
        qreal scale = item->data(SCALE).toDouble();
        qreal x = item->data(POSITION_X).toDouble() * multiply;
        qreal y = item->data(POSITION_Y).toDouble() * multiply;
        bool invert = item->data(INVERT_INTENSITY).toBool();
        int maxIntensity = item->data(MAX_INTENSITY_VALUE).toInt();
        const QGraphicsPixmapItem *pixmapItem = dynamic_cast<const QGraphicsPixmapItem*>(item);
        if (pixmapItem != nullptr) {
            engagerProgram.append(GCodeHelper::engageImageQueue(pixmapItem->pixmap().toImage(), x, y, scale, maxIntensity, invert));
        } else {
            MainSvgItem *svgItem = dynamic_cast<MainSvgItem*>(item);
            if (svgItem != nullptr) {
                engagerProgram.append(GCodeHelper::engageImageQueue(svgItem->renderPixmap().toImage(), x, y, 1,
                                                                    maxIntensity, invert));
            }
        }
    }
    totalCount = engagerProgram.count();
    passedCount = 0;
}
