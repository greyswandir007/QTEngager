#ifndef ENGAGERPROGRAM_H
#define ENGAGERPROGRAM_H

#include <QObject>
#include "icommandcreator.h"
#include "../components/mainview.h"

class EngagerProgram
{
public:
    EngagerProgram();
    EngagerProgram(const QString &filename);
    EngagerProgram(const CommandQueue &engagerProgram);
    EngagerProgram(MainView *mainView, ICommandCreator *creator);

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

    void commandQueueFromMainView(MainView *mainView, ICommandCreator *creator);
};

#endif // ENGAGERPROGRAM_H
