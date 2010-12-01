include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_ngfadapter
INCLUDEPATH +=$$NOTIFICATIONSRCDIR

PKGCONFIG += \
    dbus-glib-1

# unit test and unit
SOURCES += \
    ut_ngfadapter.cpp \
   $$NOTIFICATIONSRCDIR/ngfadapter.cpp


# unit test and unit
HEADERS += \
    ut_ngfadapter.h \
   $$NOTIFICATIONSRCDIR/ngfadapter.h

include(../common_bot.pri)
