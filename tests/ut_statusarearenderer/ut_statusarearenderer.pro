include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_statusarearenderer
INCLUDEPATH += $$SRCDIR/statusarea
PKGCONFIG += xcomposite 
PKGCONFIG += glib-2.0

# unit test and unit classes
SOURCES += \
    ut_statusarearenderer.cpp \
    $$SRCDIR/statusarea/statusarearenderer.cpp \
    $$SRCDIR/x11wrapper.cpp \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \


# unit test and unit classes
HEADERS += \
    ut_statusarearenderer.h \
    $$SRCDIR/statusarea/statusarearenderer.h \
    $$SRCDIR/statusarea/statusarea.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)
