SLIDER_PREFIX = ../../src/
INCLUDEPATH += $$SLIDER_PREFIX
QT += testlib
TEMPLATE = app
TARGET = ut_batterybusinesslogic
target.path = /usr/lib/system-ui-tests
CONFIG += dui \
    qmsystem
SOURCES += ut_batterybusinesslogic.cpp \
    $$SLIDER_PREFIX/batterybusinesslogic.cpp \
    $$SLIDER_PREFIX/systemuigconf.cpp \
    $$SLIDER_PREFIX/devicemodestub.cpp \
    $$SLIDER_PREFIX/batterystub.cpp

HEADERS += ut_batterybusinesslogic.h \
    $$SLIDER_PREFIX/batterybusinesslogic.h \
    $$SLIDER_PREFIX/systemuigconf.h \
    $$SLIDER_PREFIX/devicemodestub.h \
    $$SLIDER_PREFIX/batterystub.h

INSTALLS += target
