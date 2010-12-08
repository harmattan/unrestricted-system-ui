include(../../mconfig.pri)
INCLUDEPATH += statusarea
INCLUDEPATH += ../notifications
SOURCES += statusarea/statusarea.cpp \
    statusarea/statusareaview.cpp \
    statusarea/statusarearenderer.cpp \
    statusarea/clock.cpp \
    statusarea/clockview.cpp \
    statusarea/statusindicatorlabelview.cpp \
    statusarea/statusindicatoranimationview.cpp \
    statusarea/statusindicatoriconview.cpp \
    statusarea/statusindicator.cpp \
    statusarea/notificationstatusindicator.cpp \
    statusarea/inputmethodstatusindicatoradaptor.cpp \
    statusarea/statusarearendereradaptor.cpp
HEADERS += statusarea/statusarea.h \
    statusarea/statusareaview.h \
    statusarea/statusarearenderer.h \
    statusarea/statusareamodel.h \
    statusarea/statusareastyle.h \
    statusarea/clock.h \
    statusarea/clockview.h \
    statusarea/clockstyle.h \
    statusarea/clockmodel.h \
    statusarea/statusindicatormodel.h \
    statusarea/statusindicatorlabelview.h \
    statusarea/statusindicatorlabelstyle.h \
    statusarea/statusindicatoranimationview.h \
    statusarea/statusindicatoranimationstyle.h \
    statusarea/statusindicatoriconview.h \
    statusarea/statusindicatoriconstyle.h \
    statusarea/statusindicator.h \
    statusarea/notificationstatusindicator.h \
    statusarea/inputmethodstatusindicatoradaptor.h \
    statusarea/statusarearendereradaptor.h
MODEL_HEADERS += statusarea/statusareamodel.h \
    statusarea/clockmodel.h \
    statusarea/statusindicatormodel.h
STYLE_HEADERS += statusarea/statusareastyle.h \
    statusarea/clockstyle.h \
    statusarea/statusindicatorlabelstyle.h \
    statusarea/statusindicatoranimationstyle.h \
    statusarea/statusindicatoriconstyle.h
