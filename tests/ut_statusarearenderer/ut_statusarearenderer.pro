include(../common_top.pri)
TARGET = ut_statusarearenderer
INCLUDEPATH += $$SRCDIR/statusarea
INCLUDEPATH += $$SRCDIR/statusindicatormenu

# unit test and unit classes
SOURCES += \
    ut_statusarearenderer.cpp \
    $$SRCDIR/statusarea/statusarearenderer.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_statusarearenderer.h \
    $$SRCDIR/statusarea/statusarearenderer.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \


include(../common_bot.pri)
