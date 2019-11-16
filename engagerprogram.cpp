#include "engagerprogram.h"
#include <QFile>

EngagerProgram::EngagerProgram() {
    totalCount = 0;
    passedCount = 0;
}

EngagerProgram::EngagerProgram(QStringList engagerProgram) {
    this->engagerProgram = engagerProgram;
    totalCount = engagerProgram.count();
    passedCount = 0;
}

QStringList EngagerProgram::currentProgram() {
    return engagerProgram;
}

void EngagerProgram::setCurrentProgram(QStringList program) {
    engagerProgram = program;
    totalCount = engagerProgram.count();
    passedCount = 0;
}

void EngagerProgram::loadProgram(QString filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QString str = str.fromLatin1(file.readAll());
        engagerProgram = str.split("\x0A");
        for (int i = 0; i < engagerProgram.size(); i++) {
            if (engagerProgram[i].endsWith("\x0D")) {
                engagerProgram[i].truncate(engagerProgram[i].length()-1);
            }
            engagerProgram[i] += "\x0A";
        }
        file.close();
    }
}

void EngagerProgram::saveProgram(QString filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(engagerProgram.join("").toLatin1());
        file.close();
    }
}

void EngagerProgram::newProgram() {
    engagerProgram.clear();
    totalCount = 0;
    passedCount = 0;
}

void EngagerProgram::addCommand(QString command) {
    engagerProgram.append(command);
    totalCount++;
}

QString EngagerProgram::pullCommand() {
    QString command;
    if (!engagerProgram.isEmpty()) {
        command = engagerProgram.at(0);
        engagerProgram.removeAt(0);
        passedCount++;
    }
    return command;
}

bool EngagerProgram::hasCommand() {
    return !engagerProgram.isEmpty();
}

int EngagerProgram::totalCommandCount() {
    return totalCount;
}

int EngagerProgram::passedCommandCount() {
    return passedCount;
}

int EngagerProgram::leftCommandCount() {
    return engagerProgram.count();
}

float EngagerProgram::getCurrentProgress() {
    if (engagerProgram.isEmpty()) {
        return 0;
    } else {
        return passedCount * 100.0f / static_cast<float>(totalCount);
    }
}
