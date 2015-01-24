#-------------------------------------------------
#
# Project created by QtCreator 2014-09-12T15:39:17
#
#-------------------------------------------------

QT += core gui opengl multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pastie
TEMPLATE = app

ICON = resources/Mr-potato.icns

SOURCES += main.cpp\
    mainwindow.cpp \
    camera.cpp \
    imagelist.cpp \
    image.cpp \
    viewer.cpp \
    debugstream.cpp \
    filterlist.cpp \
    source.cpp \
    tabcalibration.cpp \
    tabimages.cpp \
    tabcamera.cpp \
    tabfilters.cpp \
    console.cpp \
    painter.cpp \
    filters/maxchannel.cpp \
    filters/normalize.cpp \
    filters/setting.cpp \
    filters/blur.cpp \
    filters/watershed.cpp \
    filters/derivative.cpp \
    robot.cpp \
    tabrobot.cpp \
    filters/pathplanner.cpp \
    filters/paddetect.cpp \
    pad.cpp \
    filters/resize.cpp \
    filters/padfilter.cpp \
    rangeslider.cpp \
    filters/rectify.cpp \
    filters/rotate.cpp

HEADERS += mainwindow.h \
    camera.h \
    imagelist.h \
    image.h \
    viewer.h \
    debugstream.h \
    filterlist.h \
    source.h \
    tabcalibration.h \
    tabimages.h \
    tabcamera.h \
    tabfilters.h \
    console.h \
    cast.h \
    painter.h \
    filters.h \
    filters/maxchannel.h \
    filters/normalize.h \
    filters/setting.h \
    filters/result.h \
    filters/blur.h \
    filters/watershed.h \
    filters/derivative.h \
    robot.h \
    tabrobot.h \
    filters/pathplanner.h \
    filters/paddetect.h \
    pad.h \
    filters/resize.h \
    qspanslider.h \
    rangeslider.h \
    range.h \
    filters/rectify.h \
    filters/rotate.h

SOURCES += filters/filter.cpp \
    filters/undistort.cpp \
    filters/threshold.cpp \
    filters/kmeans.cpp \
    filters/histequalize.cpp \
    filters/morph.cpp \
    filters/convert.cpp \
    filters/pattern.cpp \
    filters/edgedetect.cpp \
    filters/channel.cpp \
    filters/perspective.cpp

HEADERS += filters/filter.h \
    filters/kmeans.h \
    filters/threshold.h \
    filters/undistort.h \
    filters/histequalize.h \
    filters/morph.h \
    filters/convert.h \
    filters/pattern.h \
    filters/edgedetect.h \
    filters/channel.h \
    filters/perspective.h \
    filters/padfilter.h

FORMS += mainwindow.ui \
    about.ui \
    imagelist.ui \
    tabcalibration.ui \
    tabimages.ui \
    tabcamera.ui \
    tabfilters.ui \
    tabrobot.ui

# OpenCV
INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib
LIBS +=	-lopencv_core \
	-lopencv_highgui \
	-lopencv_imgproc\
	-lopencv_imgcodecs \
	-lopencv_videoio \
	-lopencv_calib3d \
	-lopencv_features2d

RESOURCES += \
    images.qrc \
    texts.qrc

OTHER_FILES +=

CONFIG += c++11
QMAKE_MAC_SDK = macosx10.10
