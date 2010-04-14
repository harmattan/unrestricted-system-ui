include(../common_top.pri)
TARGET = ut_statusarea
INCLUDEPATH += $$SRCDIR/statusarea $$SRCDIR/statusindicatormenu $$SRCDIR/notifications
PKGCONFIG += contextsubscriber-1.0

STYLE_HEADERS += $$SRCDIR/statusarea/statusareastyle.h
MODEL_HEADERS += \
    $$SRCDIR/statusarea/statusareamodel.h

# unit test and unit classes
SOURCES += \
    ut_statusarea.cpp \
    $$SRCDIR/statusarea/statusarea.cpp \
    $$SRCDIR/statusindicatormenu/pluginlist.cpp \
#    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_statusarea.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/statusarea/statusareastyle.h \
    $$SRCDIR/statusarea/statusareamodel.h \
    $$SRCDIR/applicationcontext.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuadaptor.h \
    $$SRCDIR/statusindicatormenu/pluginlist.h \
    $$SRCDIR/contextframeworkcontext.h \

# service classes
HEADERS += \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/statusindicatormenuadaptor_stub.h \
    $$SRCDIR/statusindicatormenu/notificationarea.h

include(../common_bot.pri)
