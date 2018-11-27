#-------------------------------------------------
#
# Project created by QtCreator 2013-06-06T14:14:26
#
#-------------------------------------------------
QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpServer
TEMPLATE = app


SOURCES += main.cpp\
        tcpserver.cpp \
    tcpclientsocket.cpp \
    server.cpp

HEADERS  += tcpserver.h \
    server.h \
    tcpclientsocket.h
