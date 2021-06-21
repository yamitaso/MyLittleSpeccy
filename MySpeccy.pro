QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    busdevice.cpp \
    businterface.cpp \
    businterface128.cpp \
    businterface48.cpp \
    keyboardwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    port1f.cpp \
    port7ffd.cpp \
    portfe.cpp \
    ramdevice.cpp \
    romdevice.cpp \
    screenwidget.cpp \
    3rdparty/Z80/sources/Z80.c \
    zxpushbutton.cpp

HEADERS += \
    busdevice.h \
    businterface.h \
    businterface128.h \
    businterface48.h \
    keyboardwidget.h \
    mainwindow.h \
    port1f.h \
    port7ffd.h \
    portfe.h \
    ramdevice.h \
    romdevice.h \
    screenwidget.h \
    3rdparty/Z80/API/emulation/CPU/Z80.h \
    zxpushbutton.h

FORMS += \
    mainwindow.ui

OTHER_FILES += \
    files/48.rom \
    files/128.rom \

INCLUDEPATH += \
    3rdparty/Z/API \
    3rdparty/Z80/API

DEFINES += \
    CPU_Z80_STATIC
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    files.qrc
