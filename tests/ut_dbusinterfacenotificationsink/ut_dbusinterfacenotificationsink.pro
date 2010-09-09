include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_dbusinterfacenotificationsink
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

QMAKE_CXXFLAGS += -fno-inline

# unit test and unit
SOURCES += \
    ut_dbusinterfacenotificationsink.cpp \
    $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_dbusinterfacenotificationsink.h \
    $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsink.h \
    $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsinkadaptor.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \


include(../common_bot.pri)
