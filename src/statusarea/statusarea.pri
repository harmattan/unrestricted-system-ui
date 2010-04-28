include(../../mconfig.pri)

INCLUDEPATH += statusarea
INCLUDEPATH += ../notifications

SOURCES += statusarea/statusarea.cpp \
    statusarea/statusareaview.cpp \
    statusarea/statusarearenderer.cpp \
    statusarea/clock.cpp \
    statusarea/clockview.cpp \
    statusarea/notifier.cpp \
    statusarea/statusindicatorlabelview.cpp \
    statusarea/statusindicatorimageview.cpp \
    statusarea/statusindicator.cpp

HEADERS += statusarea/statusarea.h \
    statusarea/statusareaview.h \
    statusarea/statusarearenderer.h \
    statusarea/statusareamodel.h \
    statusarea/statusareastyle.h \
    statusarea/clock.h \
    statusarea/clockview.h \
    statusarea/clockstyle.h \
    statusarea/clockmodel.h \
    statusarea/notifier.h \
    statusarea/statusindicatormodel.h \
    statusarea/statusindicatorlabelview.h \
    statusarea/statusindicatorlabelstyle.h \
    statusarea/statusindicatorimageview.h \
    statusarea/statusindicatorimagestyle.h \
    statusarea/statusindicator.h

MODEL_HEADERS +=     statusarea/statusareamodel.h \
    statusarea/clockmodel.h \
    statusarea/statusindicatormodel.h

STYLE_HEADERS += statusarea/statusareastyle.h \
    statusarea/clockstyle.h \
    statusarea/statusindicatorlabelstyle.h \
    statusarea/statusindicatorimagestyle.h


