#-------------------------------------------------
#
# Project created by QtCreator 2018-08-18T23:19:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestGmpLib
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#LIBS += $$PWD/lib/libgmp.so.1 \
#    $$PWD/lib/libpbc.so.1
#LIBS += /usr/local/lib/lgmp \
#    /usr/local/lib/lpbc
DESTDIR += $$PWD/000_ExePath
SOURCES += \
        main.cpp \
        widget.cpp

HEADERS += \
        widget.h \
    HeaderFile/gmp.h \
    HeaderFile/pbc.h \
    HeaderFile/pbc_a_param.h \
    HeaderFile/pbc_a1_param.h \
    HeaderFile/pbc_curve.h \
    HeaderFile/pbc_d_param.h \
    HeaderFile/pbc_e_param.h \
    HeaderFile/pbc_f_param.h \
    HeaderFile/pbc_field.h \
    HeaderFile/pbc_fieldquadratic.h \
    HeaderFile/pbc_fp.h \
    HeaderFile/pbc_g_param.h \
    HeaderFile/pbc_hilbert.h \
    HeaderFile/pbc_i_param.h \
    HeaderFile/pbc_memory.h \
    HeaderFile/pbc_mnt.h \
    HeaderFile/pbc_multiz.h \
    HeaderFile/pbc_pairing.h \
    HeaderFile/pbc_param.h \
    HeaderFile/pbc_poly.h \
    HeaderFile/pbc_random.h \
    HeaderFile/pbc_singular.h \
    HeaderFile/pbc_ternary_extension_field.h \
    HeaderFile/pbc_test.h \
    HeaderFile/pbc_utils.h \
    HeaderFile/pbc_z.h

FORMS += \
        widget.ui

unix:!macx: LIBS += -L$$PWD/lib/ -lgmp
unix:!macx: LIBS += -L$$PWD/lib/ -lpbc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

