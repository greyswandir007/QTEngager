#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QTranslator translator;
    translator.load("qtengager_ru");
    QApplication a(argc, argv);
    a.installTranslator(&translator);
    MainWindow w;
    w.show();

    return a.exec();
}
