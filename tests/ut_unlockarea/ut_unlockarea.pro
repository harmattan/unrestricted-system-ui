include(../coverage.pri)
include(../common_top.pri)
INCLUDEPATH += $$SRCDIR/lockscreen
 
TARGET = ut_unlockarea

# unit test and unit
SOURCES += \
    ut_unlockarea.cpp \
    $$SRCDIR/lockscreen/unlockarea.cpp 

# unit test and unit
HEADERS += \
    ut_unlockarea.h \
    $$SRCDIR/lockscreen/unlockarea.h

include(../common_bot.pri)
