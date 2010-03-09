include(../common_top.pri)
TARGET = ut_statusindicator

INCLUDEPATH += $$SRCDIR/statusarea

STYLE_HEADERS += $$SRCDIR/statusarea/statusindicatorimagestyle.h \
    $$SRCDIR/statusarea/statusindicatorlabelstyle.h
MODEL_HEADERS += $$SRCDIR/statusarea/statusindicatormodel.h

# unit test and unit classes
SOURCES += ut_statusindicator.cpp \
    $$SRCDIR/statusarea/statusindicator.cpp \
    $$SRCDIR/statusarea/statusindicatorimageview.cpp \
    $$SRCDIR/statusarea/statusindicatorlabelview.cpp

# helper classes
SOURCES +=  \
    $$STUBSDIR/testcontextitem.cpp

# unit test and unit classes
HEADERS += ut_statusindicator.h \
    $$SRCDIR/statusarea/statusindicator.h \
    $$SRCDIR/statusarea/applicationcontext.h  \
    $$SRCDIR/statusarea/statusindicatorimagestyle.h \
    $$SRCDIR/statusarea/statusindicatorlabelstyle.h \
    $$SRCDIR/statusarea/statusindicatormodel.h \

# helper classes
HEADERS +=  \
    $$STUBSDIR/testcontextitem.h

# base classes
# HEADERS += \
# service classes
HEADERS += $$SRCDIR/statusarea/statusindicatorimageview.h \
    $$SRCDIR/statusarea/statusindicatorlabelview.h
