include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationstatusindicator

INCLUDEPATH += $$SRCDIR/statusarea $$SRCDIR/notifications $$SRCDIR

MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit classes
HEADERS += ut_notificationstatusindicator.h \
    $$SRCDIR/notifications/notificationstatusindicator.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/statusarea/inputmethodstatusindicatoradaptor.h \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notifiernotificationsink.h \
    $$SRCDIR/notifications/mcompositornotificationsink.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/statusarea/statusindicatormodel.h \
    $$SRCDIR/notifications/notificationmanager.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/eventtypestore.h

# unit test and unit classes
SOURCES += ut_notificationstatusindicator.cpp \
    $$SRCDIR/notifications/notificationstatusindicator.cpp \
    $$SRCDIR/statusarea/statusindicator.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp

# helper classes
SOURCES +=  \
    $$STUBSDIR/stubbase.cpp
