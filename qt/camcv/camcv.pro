#-------------------------------------------------
#
# Project created by QtCreator 2013-08-09T13:17:23
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = camcv
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    RaspiPreview.c \
    RaspiCLI.c \
    RaspiCamControl.c \
    camcv.c

INCLUDEPATH += \opt\vc\host_applications\linux\libs\bcm_host\include \
    \opt\vc\interface\vcos \
    \opt\vc \
    \opt\vc\interface\vcos\pthreads \
    \opt\vc\interface\vmcs_host\linux

LIBS += -L\opt\vc\lib \
    -lmmal_core \
    -lmmal_util \
    -lmmal_vc_client \
    -lvcos \
    -lbcm_host

HEADERS += \
    RaspiPreview.h \
    RaspiCLI.h \
    RaspiCamControl.h
