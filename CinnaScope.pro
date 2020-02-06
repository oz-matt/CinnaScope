#-------------------------------------------------
#
# Project created by QtCreator 2019-12-03T13:55:03
#
#-------------------------------------------------

QT       += core gui
CONFIG   += qwt full

INCLUDEPATH += /usr/local/qwt-6.1.5-svn/include
LIBS += -L/usr/local/qwt-6.1.5-svn/lib -lqwt -L/home/matt/CinnaScope/pcie_lib -ldl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CinnaScope
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


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    plot.cpp \
    curvedata.cpp \
    signaldata.cpp \
    samplingthread.cpp \
    cinnafixedknob.cpp \
    cinnaroundscaledraw.cpp \
    cinnastate.cpp \
    knob.cpp \
    wheelbox.cpp \
    oscwidget.cpp \
    cinnapcieinterface.cpp

HEADERS += \
    dummydata.h \
    mainwindow.h \
    cinnafixedknob.h \
    cinnaroundscaledraw.h \
    cinnastate.h \
    plot.h \
    curvedata.h \
    signaldata.h \
    samplingthread.h \
    knob.h \
    wheelbox.h \
    oscwidget.h \
    cinnastyle.h \
    cinna.h \
    cinnapcieinterface.h

    full {
        SOURCES += \
        pcie_lib/PCIE.c

        HEADERS += \
        pcie_lib/PCIE.h \
        pcie_lib/TERASIC_PCIE.h \

        INCLUDEPATH += pcie_lib/

        #QMAKE_CXXFLAGS += -ldl
        QMAKE_LFLAGS += -I/home/matt/CinnaScope/pcie_lib/terasic_pcie_qsys.so -ldl

        #LIBS += /home/matt/CinnaScope/pcie_lib/terasic_pcie_qsys.so
    }Z

FORMS += \
        mainwindow.ui
