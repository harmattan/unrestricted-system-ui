SLIDER_PREFIX = ../../src/unlocksliderwidget
UNLOCK_PREFIX = ../../src

INCLUDEPATH += $$SLIDER_PREFIX
QT += testlib
TEMPLATE = app
TARGET = ut_lockscreenui
target.path = /usr/lib/system-ui-tests

CONFIG += \
    dui \
    silent

SOURCES += ut_lockscreenui.cpp \
    $$SLIDER_PREFIX/unlockslidermodel.cpp \
    $$SLIDER_PREFIX/unlocksliderview.cpp \
    $$SLIDER_PREFIX/unlockslider.cpp \
    $$UNLOCK_PREFIX/lockscreenui.cpp 

HEADERS += ut_lockscreenui.h \
    $$SLIDER_PREFIX/unlockslidermodel.h \
    $$SLIDER_PREFIX/unlocksliderview.h \
    $$SLIDER_PREFIX/unlocksliderstyle.h \
    $$SLIDER_PREFIX/unlockslider.h \
    $$UNLOCK_PREFIX/lockscreenui.h 

MODEL_HEADERS += $$SLIDER_PREFIX/unlockslidermodel.h
STYLE_HEADERS += $$SLIDER_PREFIX/unlocksliderstyle.h

INSTALLS += target
