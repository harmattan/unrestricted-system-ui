include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_eventtypestore
INCLUDEPATH +=$$NOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_eventtypestore.cpp \
   $$NOTIFICATIONSRCDIR/eventtypestore.cpp

# unit test and unit
HEADERS += \
    ut_eventtypestore.h \
   $$NOTIFICATIONSRCDIR/eventtypestore.h \

include(../common_bot.pri)
