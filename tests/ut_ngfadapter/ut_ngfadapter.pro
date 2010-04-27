include(../common_top.pri)
TARGET = ut_ngfadapter
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_ngfadapter.cpp \
    $$SRCDIR/notifications/ngfadapter.cpp


# unit test and unit
HEADERS += \
    ut_ngfadapter.h \
    $$SRCDIR/notifications/ngfadapter.h

include(../common_bot.pri)
