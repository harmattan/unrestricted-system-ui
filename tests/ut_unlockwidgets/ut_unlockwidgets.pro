include(../coverage.pri)
include(../common_top.pri)
INCLUDEPATH += $$SRCDIR/lockscreen
 
TARGET = ut_unlockwidgets

# unit test and unit
SOURCES += \
    ut_unlockwidgets.cpp \
    $$SRCDIR/lockscreen/unlockwidgets.cpp \
    $$SRCDIR/debug.cpp


# unit test and unit
HEADERS += \
    ut_unlockwidgets.h \
    $$SRCDIR/lockscreen/unlockwidgets.h \
    $$SRCDIR/debug.h

include(../common_bot.pri)
