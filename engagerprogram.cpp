#include "engagerprogram.h"
#include <QFile>

EngagerProgram::EngagerProgram() {
    totalCount = 0;
    passedCount = 0;
}

EngagerProgram::EngagerProgram(const QStringList &engagerProgram) {
    this->engagerProgram = engagerProgram;
    totalCount = engagerProgram.count();
    passedCount = 0;
}

QStringList EngagerProgram::currentProgram() const {
    return engagerProgram;
}

void EngagerProgram::setCurrentProgram(const QStringList &program) {
    engagerProgram = program;
    totalCount = engagerProgram.count();
    passedCount = 0;
}

void EngagerProgram::loadProgram(const QString &filename) {
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

void EngagerProgram::saveProgram(const QString &filename) {
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

void EngagerProgram::addCommand(const QString &command) {
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
    if (engagerProgram.isEmpty()) {
        return 0;
    } else {
        return passedCount * 100.0f / static_cast<float>(totalCount);
    }
}
