QT       += core gui
include(../../../QTGifimage-master/src/gifimage/qtgifimage.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += widgets opengl openglwidgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += GL_SILENCE_DEPRECATION

SOURCES += \
    ../../afins.c \
    ../../parse.c \
    geometry.cpp \
    main.cpp \
    mainwindow.cpp \
    widget.cpp

HEADERS += \
    ../../afins.h \
    ../../parse.h \
    geometry.h \
    mainwindow.h \
    widget.h

TRANSLATIONS += \
    QT_3D_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    ../img/wolf_bg.jpeg

RESOURCES += \
    shaders.qrc

FORMS += \
    mainwindow.ui

ICON = freedom.icns
