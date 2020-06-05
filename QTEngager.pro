#-------------------------------------------------
#
# Project created by QtCreator 2019-11-01T19:23:40
#
#-------------------------------------------------

QT       += core gui serialport xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTEngager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    components/graphicsviewruler.cpp \
    components/itempropertiesdialog.cpp \
    components/itempropertiesmodel.cpp \
    components/mainview.cpp \
    engager/engagercommand.cpp \
    engager/engagercontroller.cpp \
    engager/engagerprogram.cpp \
    engager/logdialog.cpp \
    components/graphicitems/mainpixmapitem.cpp \
    components/graphicitems/mainsvgitem.cpp \
    engager/gcodecommands.cpp \
    engager/commandcreator.cpp

HEADERS += \
    mainwindow.h \
    components/graphicsviewruler.h \
    components/itempropertiesdialog.h \
    components/itempropertiesmodel.h \
    components/mainview.h \
    engager/engagercommand.h \
    engager/engagercontroller.h \
    engager/engagerprogram.h \
    engager/logdialog.h \
    components/graphicitems/mainpixmapitem.h \
    components/graphicitems/maingraphicsitem.h \
    components/graphicitems/mainsvgitem.h \
    engager/gcodecommands.h \
    engager/commandcreator.h \
    engager/igcodecommands.h \
    engager/icommandcreator.h

FORMS += mainwindow.ui \
    components/itempropertiesdialog.ui \
    engager/logdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += qtengager.qrc

TRANSLATIONS += qtengager_ru.ts
