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

QMAKE_RPATHDIR += /home/gromchen/rpi/usr/lib \
    /home/gromchen/rpi/usr/lib/arm-linux-gnueabihf \
    /home/gromchen/rpi/usr/local/lib

INCLUDEPATH += /home/gromchen/rpi/opt/vc/host_applications/linux/libs/bcm_host/include \
    /home/gromchen/rpi/opt/vc/interface/vcos \
    /home/gromchen/rpi/opt/vc \
    /home/gromchen/rpi/opt/vc/interface/vcos/pthreads \
    /home/gromchen/rpi/opt/vc/interface/vmcs_host/linux \
    /home/gromchen/rpi/include \
    /home/gromchen/git_hub/fall-detector/fall_detector_lib

LIBS += -L/home/gromchen/rpi/opt/vc/lib \
    -lmmal_core \
    -lmmal_util \
    -lmmal_vc_client \
    -lvcos \
    -lbcm_host

LIBS += -L/home/gromchen/rpi/usr/lib \
    -lopencv_highgui \
    -lopencv_core \
    -lopencv_features2d \
    -lopencv_calib3d \
    -lopencv_video \
    -lopencv_imgproc

LIBS += -L/home/gromchen/Qt_builds/fall_detector_lib/rpi/release \
    -lfall_detector_lib

target.path = ./Qt_deploy/
INSTALLS += target
