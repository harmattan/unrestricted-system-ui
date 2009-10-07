INCLUDEPATH += ..
QT += testlib
TEMPLATE = app
TARGET = ut_unlockslider
CONFIG += dui

SOURCES += ut_unlockslider.cpp \
    ../unlockslidermodel.cpp \
    ../unlocksliderview.cpp \
    ../unlockslider.cpp

HEADERS += ut_unlockslider.h \
    ../unlockslidermodel.h \
    ../unlocksliderview.h \
    ../unlocksliderstyle.h \
    ../unlockslider.h

MODEL_HEADERS += ../unlockslidermodel.h
STYLE_HEADERS += ../unlocksliderstyle.h

#    unlockslider.cpp \
#    unlockslidermodel.cpp \
#    unlocksliderview.cpp
#HEADERS += unlockslider.h \
#    unlocksliderstyle.h \
#    unlockslidermodel.h \
#    unlocksliderview.h
