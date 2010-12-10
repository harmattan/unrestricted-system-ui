include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationstatusindicator

INCLUDEPATH += $$SRCDIR/statusarea $$NOTIFICATIONSRCDIR $$SRCDIR $$LIBNOTIFICATIONSRCDIR

MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit classes
HEADERS += ut_notificationstatusindicator.h \
    $$SRCDIR/statusarea/notificationstatusindicator.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/statusarea/statusindicatormodel.h \

# unit test and unit classes
SOURCES += ut_notificationstatusindicator.cpp \
   $$SRCDIR/statusarea/notificationstatusindicator.cpp \
   $$SRCDIR/statusarea/statusindicator.cpp \

# helper classes
SOURCES +=  \
    $$STUBSDIR/stubbase.cpp
