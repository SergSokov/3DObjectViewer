#QT       += core gui opengl openglwidgets
QT       += core gui widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += core gui widgets opengl

TARGET = 3dObjectViewer
TEMPLATE = app


#CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    material.cpp \
    materiallibrary.cpp \
    objectengine3d.cpp \
    objviewer.cpp \
    simpleobject3d.cpp

HEADERS += \
    mainwindow.h \
    material.h \
    materiallibrary.h \
    objectengine3d.h \
    objviewer.h \
    simpleobject3d.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc \
    textures.qrc

CONFIG += windows
INSTALLS += target

