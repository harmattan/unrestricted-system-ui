include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusindicatormenuverticalview
INCLUDEPATH += $$SRCDIR/statusindicatormenu $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

# unit test and unit classes
SOURCES += \
    ut_statusindicatormenuverticalview.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenuverticalview.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenu.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_statusindicatormenuverticalview.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuverticalview.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenu.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindowstyle.h

STYLE_HEADERS += $$SRCDIR/statusindicatormenu/statusindicatormenuwindowstyle.h

include(../common_bot.pri)
