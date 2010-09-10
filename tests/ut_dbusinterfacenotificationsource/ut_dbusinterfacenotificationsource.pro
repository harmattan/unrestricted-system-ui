include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_dbusinterfacenotificationsource
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_dbusinterfacenotificationsource.cpp \
   $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsource.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationsource.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_dbusinterfacenotificationsource.h \
   $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsource.h \
   $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsourceadaptor.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.h

include(../common_bot.pri)
