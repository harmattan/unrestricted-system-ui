include(../common_top.pri)
TARGET = ut_statusareaview
INCLUDEPATH += $$SRCDIR/statusarea

STYLE_HEADERS += $$SRCDIR/statusarea/statusareastyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusareamodel.h

# unit test and unit
SOURCES += \
    ut_statusareaview.cpp \
    $$SRCDIR/statusarea/statusareaview.cpp \
    $$SRCDIR/statusarea/statusarea.cpp

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
    $$SRCDIR/statusarea/clock.h

include(../common_bot.pri)
