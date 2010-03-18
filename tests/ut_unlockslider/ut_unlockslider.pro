include(../common_top.pri)
TARGET = ut_unlockslider

SLIDER_PREFIX = ../../src/unlocksliderwidget
INCLUDEPATH += $$SLIDER_PREFIX

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

include(../common_bot.pri)
