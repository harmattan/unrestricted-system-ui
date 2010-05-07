include(../common_top.pri)
TARGET = ut_clockview

INCLUDEPATH += $$SRCDIR/statusarea

STYLE_HEADERS += $$SRCDIR/statusarea/clockstyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/clockmodel.h

# unit test and unit
SOURCES += \
    ut_clockview.cpp \
    $$SRCDIR/statusarea/clockview.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/testcontextitem.cpp

# unit test and unit
HEADERS += \
    ut_clockview.h \
    $$SRCDIR/statusarea/clockview.h \
    $$SRCDIR/statusarea/clockmodel.h \
    $$SRCDIR/statusarea/clockstyle.h \
    $$SRCDIR/statusarea/clock.h \
    $$SRCDIR/applicationcontext.h

# service classes
HEADERS += \
    $$STUBSDIR/testcontextitem.h

include(../common_bot.pri)
