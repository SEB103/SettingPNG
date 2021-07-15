#Project: Setting PNG

#FILE: SettingPNG.pro
#AUTHOR: SEB

#OVERVIEW
#========
#QMake project file

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TARGET = SettingPNG
RC_ICONS = png/png.ico

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    png.qrc \
    svg.qrc
