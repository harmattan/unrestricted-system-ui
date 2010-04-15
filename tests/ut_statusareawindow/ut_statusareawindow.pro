include(../common_top.pri)
TARGET = ut_statusareawindow
INCLUDEPATH += $$SRCDIR/statusarea
INCLUDEPATH += $$SRCDIR/statusindicatormenu

# unit test and unit classes
SOURCES += \
    ut_statusareawindow.cpp \
    $$SRCDIR/statusarea/statusareawindow.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_statusareawindow.h \
    $$SRCDIR/statusarea/statusareawindow.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \


include(../common_bot.pri)
