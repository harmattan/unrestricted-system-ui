include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationstatusindicator

INCLUDEPATH += $$SRCDIR/statusarea $$NOTIFICATIONSRCDIR $$SRCDIR $$LIBNOTIFICATIONSRCDIR

MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit classes
HEADERS += ut_notificationstatusindicator.h \
   $$NOTIFICATIONSRCDIR/notificationstatusindicator.h \
   $$SRCDIR/statusarea/statusindicator.h \
   $$SRCDIR/statusarea/inputmethodstatusindicatoradaptor.h \
   $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
   $$NOTIFICATIONSRCDIR/notifiernotificationsink.h \
   $$NOTIFICATIONSRCDIR/mcompositornotificationsink.h \
   $$NOTIFICATIONSRCDIR/widgetnotificationsink.h \
   $$SRCDIR/sysuid.h \
   $$SRCDIR/statusarea/statusindicatormodel.h \
   $$NOTIFICATIONSRCDIR/notificationmanager.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
   $$LIBNOTIFICATIONSRCDIR/notification.h \
   $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
   $$NOTIFICATIONSRCDIR/eventtypestore.h

# unit test and unit classes
SOURCES += ut_notificationstatusindicator.cpp \
   $$NOTIFICATIONSRCDIR/notificationstatusindicator.cpp \
   $$SRCDIR/statusarea/statusindicator.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp

# helper classes
SOURCES +=  \
    $$STUBSDIR/stubbase.cpp
