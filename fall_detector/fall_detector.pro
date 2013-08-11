#-------------------------------------------------
#
# Project created by QtCreator 2013-08-08T13:53:02
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = fall_detector
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    application.cpp \
    video_processor.cpp \
    helpers.cpp \
    video_data.cpp

HEADERS += \
    application.h \
    video_processor.h \
    helpers.h \
    video_data.h

win32 {
    INCLUDEPATH += $(OPENCV_DIR)\include \
        $(BOOST_DIR)\include

    LIBS += -L$(OPENCV_DIR)\bin \
        -lopencv_highgui246d \
        -lopencv_core246d \
        -lopencv_features2d246d \
        -lopencv_calib3d246d \
        -lopencv_video246d \
        -lopencv_imgproc246d \
        -lopencv_objdetect246d

    LIBS += -L$(BOOST_DIR)\lib \
        -lboost_thread-mgw47-mt-d-1_54 \
        -lboost_chrono-mgw47-d-1_54 \
        -lboost_system-mgw47-d-1_54
}

unix {
    INCLUDEPATH += /usr/local/include \
        /usr/include

    LIBS += -L/usr/lib \
        -lopencv_highgui \
        -lopencv_core \
        -lopencv_features2d \
        -lopencv_calib3d \
        -lopencv_video \
        -lopencv_imgproc

    LIBS += -L/usr/local/lib \
        -lboost_thread \
        -lboost_chrono
}
