SLIDER_PREFIX = ../../src/unlocksliderwidget
INCLUDEPATH += $$SLIDER_PREFIX
QT += testlib
TEMPLATE = app
TARGET = ut_unlockslider
target.path = /usr/lib/system-ui-tests
CONFIG += dui

SOURCES += ut_unlockslider.cpp \
    $$SLIDER_PREFIX/unlockslidermodel.cpp \
    $$SLIDER_PREFIX/unlocksliderview.cpp \
    $$SLIDER_PREFIX/unlockslider.cpp

HEADERS += ut_unlockslider.h \
    $$SLIDER_PREFIX/unlockslidermodel.h \
    $$SLIDER_PREFIX/unlocksliderview.h \
    $$SLIDER_PREFIX/unlocksliderstyle.h \
    $$SLIDER_PREFIX/unlockslider.h

MODEL_HEADERS += $$SLIDER_PREFIX/unlockslidermodel.h
STYLE_HEADERS += $$SLIDER_PREFIX/unlocksliderstyle.h

target.files += ut_unlockslider

INSTALLS += target
