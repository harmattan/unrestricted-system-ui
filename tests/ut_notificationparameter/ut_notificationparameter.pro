include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationparameter
INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_notificationparameter.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
    ut_notificationparameter.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.h

# base classes
HEADERS += \


# service classes
HEADERS += \


include(../common_bot.pri)
