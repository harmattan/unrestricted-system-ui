include(../common_top.pri)
TARGET = ut_lockscreenui

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs
SLIDER_PREFIX = ../../src/unlocksliderwidget
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX \
               $$SLIDER_PREFIX 

SOURCES += ut_lockscreenui.cpp \
    $$SLIDER_PREFIX/unlockslidermodel.cpp \
    $$SLIDER_PREFIX/unlocksliderview.cpp \
    $$SLIDER_PREFIX/unlockslider.cpp \
    $$SRC_PREFIX/lockscreenui.cpp  \
    $$STUB_PREFIX/stubbase.cpp

HEADERS += ut_lockscreenui.h \
    $$SLIDER_PREFIX/unlockslidermodel.h \
    $$SLIDER_PREFIX/unlocksliderview.h \
    $$SLIDER_PREFIX/unlocksliderstyle.h \
    $$SLIDER_PREFIX/unlockslider.h \
    $$SRC_PREFIX/lockscreenui.h \
    $$SRC_PREFIX/sysuid.h 

MODEL_HEADERS += $$SLIDER_PREFIX/unlockslidermodel.h
STYLE_HEADERS += $$SLIDER_PREFIX/unlocksliderstyle.h

include(../common_bot.pri)

