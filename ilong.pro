#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T14:37:35
#
#-------------------------------------------------

#QT       -= gui
QT       += widgets network sql

TARGET = ilong
TEMPLATE = lib
DESTDIR = ../bin
DEFINES += ILONG_LIBRARY

SOURCES += ILong.cpp \
    Map.cpp \
    Network.cpp \
    ILoveChina.cpp \
    SQLExcute.cpp \
    Manager.cpp \
    Layer.cpp \
    Geometry.cpp \
    GeoPie.cpp \
    GeoRect.cpp \
    GeoCircle.cpp \
    GeoStar.cpp \
    GeoTri.cpp \
    GeoPolygon.cpp

HEADERS += ILong.h\
        ilong_global.h \
    Map.h \
    Network.h \
    ILoveChina.h \
    SQLExcute.h \
    Manager.h \
    Layer.h \
    Geometry.h \
    GeoPie.h \
    GeoRect.h \
    GeoCircle.h \
    GeoStar.h \
    GeoTri.h \
    GeoPolygon.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS +=
