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

HEADERS += \
    RaspiPreview.h \
    RaspiCLI.h \
    RaspiCamControl.h

INCLUDEPATH += /home/gromchen/rpi/opt/vc/host_applications/linux/libs/bcm_host/include \
    /home/gromchen/rpi/opt/vc/interface/vcos \
    /home/gromchen/rpi/opt/vc \
    /home/gromchen/rpi/opt/vc/interface/vcos/pthreads \
    /home/gromchen/rpi/opt/vc/interface/vmcs_host/linux

LIBS += -L/home/gromchen/rpi/opt/vc/lib \
    -lmmal_core \
    -lmmal_util \
    -lmmal_vc_client \
    -lvcos \
    -lbcm_host

target.path = ./Qt_deploy/
INSTALLS += target
