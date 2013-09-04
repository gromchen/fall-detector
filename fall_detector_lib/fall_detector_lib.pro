#-------------------------------------------------
#
# Project created by QtCreator 2013-09-04T10:32:48
#
#-------------------------------------------------

QT += core
QT       -= gui

TARGET = fall_detector_lib
TEMPLATE = lib

DEFINES += FALL_DETECTOR_LIB_LIBRARY

SOURCES += fall_detector_lib.cpp \
    interval_processor.cpp \
    interval_info.cpp \
    helpers.cpp \
    frame_info.cpp \
    finite_state_machine.cpp \
    feature.cpp \
    feature_collection.cpp \
    data_collector.cpp

HEADERS += fall_detector_lib.h\
        fall_detector_lib_global.h \
    interval_processor.h \
    interval_info.h \
    helpers.h \
    frame_info.h \
    finite_state_machine.h \
    feature.h \
    feature_collection.h \
    data_collector.h

#unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}

linux-g++ {
    INCLUDEPATH += /usr/local/include

    LIBS += -L/usr/local/lib \
        -lboost_thread \
        -lboost_chrono \
        -lopencv_highgui \
        -lopencv_core \
        -lopencv_features2d \
        -lopencv_calib3d \
        -lopencv_video \
        -lopencv_imgproc
}

linux-rasp-pi-g++ {
    QMAKE_RPATHDIR += /home/gromchen/rpi/usr/lib \
        /home/gromchen/rpi/usr/lib/arm-linux-gnueabihf \
        /home/gromchen/rpi/usr/local/lib

    INCLUDEPATH += /home/gromchen/rpi/include

    LIBS += -L/home/gromchen/rpi/usr/lib \
        -lopencv_highgui \
        -lopencv_core \
        -lopencv_features2d \
        -lopencv_calib3d \
        -lopencv_video \
        -lopencv_imgproc

    LIBS += -L/home/gromchen/rpi/usr/local/lib \
        -lboost_system \
        -lboost_thread \
        -lboost_chrono

     target.path = ./Qt_deploy/
     INSTALLS += target
}
