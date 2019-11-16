#ifndef ENGAGERPROGRAM_H
#define ENGAGERPROGRAM_H

#include <QObject>

class EngagerProgram
{
public:
    EngagerProgram();
    EngagerProgram(QStringList engagerProgram);
    QStringList currentProgram();
    void setCurrentProgram(QStringList engagerProgram);
    void loadProgram(QString filename);
    void saveProgram(QString filename);
    void newProgram();

    void addCommand(QString command);

    QString pullCommand();
    bool hasCommand();

    int totalCommandCount();
    int passedCommandCount();
    int leftCommandCount();

    float getCurrentProgress();

private:
    QStringList engagerProgram;
    int totalCount;
    int passedCount;
};

#endif // ENGAGERPROGRAM_H
