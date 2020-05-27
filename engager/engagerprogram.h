#ifndef ENGAGERPROGRAM_H
#define ENGAGERPROGRAM_H

#include <QObject>
#include "gcodehelper.h"

class EngagerProgram
{
public:
    EngagerProgram();
    EngagerProgram(const CommandQueue &engagerProgram);
    CommandQueue currentProgram() const;
    void setCurrentProgram(const CommandQueue &engagerProgram);
    void loadProgram(const QString &filename);
    void saveProgram(const QString &filename);
    void newProgram();

    void addCommand(const CommandQueue &command);

    EngagerCommand pullCommand();
    bool hasCommand() const;

    int totalCommandCount() const;
    int passedCommandCount() const;
    int leftCommandCount() const;

    float getCurrentProgress() const;

private:
    CommandQueue engagerProgram;
    int totalCount;
    int passedCount;
};

#endif // ENGAGERPROGRAM_H
