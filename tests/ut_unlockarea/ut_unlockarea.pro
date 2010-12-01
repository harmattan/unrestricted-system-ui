include(../coverage.pri)
include(../common_top.pri)
INCLUDEPATH += $$ROOTSRCDIR/extensions/screenlock
 
TARGET = ut_unlockarea

# unit test and unit
SOURCES += \
    ut_unlockarea.cpp \
    $$ROOTSRCDIR/extensions/screenlock/unlockarea.cpp 

# unit test and unit
HEADERS += \
    ut_unlockarea.h \
    $$ROOTSRCDIR/extensions/screenlock/unlockarea.h

include(../common_bot.pri)
