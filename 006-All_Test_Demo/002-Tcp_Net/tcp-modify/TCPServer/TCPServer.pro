#-------------------------------------------------
#
# Project created by QtCreator 2015-07-13T16:11:34
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCPServer
TEMPLATE = app


SOURCES += main.cpp\
        tcpserver.cpp \
    tcpclientsocket.cpp \
    server.cpp

HEADERS  += tcpserver.h \
    tcpclientsocket.h \
    server.h
