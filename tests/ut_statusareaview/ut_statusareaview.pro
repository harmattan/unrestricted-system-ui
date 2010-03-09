include(../common_top.pri)
TARGET = ut_statusareaview
INCLUDEPATH += $$SRCDIR/statusarea $$SRCDIR/statusindicatormenu
INCLUDEPATH += /usr/include/contextsubscriber

STYLE_HEADERS += $$SRCDIR/statusarea/statusareastyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusareamodel.h

# unit test and unit
SOURCES += \
    ut_statusareaview.cpp \
    $$SRCDIR/statusarea/statusareaview.cpp \
    $$SRCDIR/statusarea/statusarea.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.cpp \
    $$SRCDIR/statusindicatormenu/pluginlist.cpp

# service classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_statusareaview.h \
    $$SRCDIR/statusarea/statusareastyle.h \
    $$SRCDIR/statusarea/statusareamodel.h \
    $$SRCDIR/statusarea/statusareaview.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/statusarea/notifier.h \
    $$SRCDIR/statusarea/clock.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \
    $$SRCDIR/statusindicatormenu/pluginlist.h

include(../common_bot.pri)
