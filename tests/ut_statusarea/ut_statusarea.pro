include(../common_top.pri)
TARGET = ut_statusarea
INCLUDEPATH += $$SRCDIR/statusarea  $$SRCDIR/statusindicatormenu

STYLE_HEADERS += $$SRCDIR/statusarea/statusareastyle.h
MODEL_HEADERS += \
    $$SRCDIR/statusarea/statusareamodel.h

# unit test and unit classes
SOURCES += \
    ut_statusarea.cpp \
    $$SRCDIR/statusarea/statusarea.cpp \
    $$SRCDIR/statusindicatormenu/pluginlist.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.cpp

# unit test and unit classes
HEADERS += \
    ut_statusarea.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/statusarea/statusareastyle.h \
    $$SRCDIR/statusarea/statusareamodel.h \
    $$SRCDIR/statusarea/applicationcontext.h \
    $$SRCDIR/statusarea/contextframeworkcontext.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \
    $$SRCDIR/statusindicatormenu/pluginlist.h

include(../common_bot.pri)
