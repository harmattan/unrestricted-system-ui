include(../../mconfig.pri)

INCLUDEPATH += statusindicatormenu

SOURCES += \
    statusindicatormenu/statusindicatormenubusinesslogic.cpp \
    statusindicatormenu/statusindicatormenuwindow.cpp \
    statusindicatormenu/statusindicatormenuadaptor.cpp \
    statusindicatormenu/notificationarea.cpp \
    statusindicatormenu/notificationareaview.cpp \
    statusindicatormenu/statusindicatormenu.cpp \
    statusindicatormenu/statusindicatormenudropdownview.cpp \
    statusindicatormenu/statusindicatormenuverticalview.cpp

HEADERS += \
    statusindicatormenu/statusindicatormenubusinesslogic.h \
    statusindicatormenu/statusindicatormenuwindow.h \
    statusindicatormenu/statusindicatormenuadaptor.h \
    statusindicatormenu/notificationarea.h \
    statusindicatormenu/notificationareamodel.h \
    statusindicatormenu/notificationareaview.h \
    statusindicatormenu/notificationareastyle.h \
    statusindicatormenu/statusindicatormenustyle.h \
    statusindicatormenu/statusindicatormenu.h \
    statusindicatormenu/statusindicatormenudropdownview.h \
    statusindicatormenu/statusindicatormenuverticalview.h

MODEL_HEADERS += statusindicatormenu/notificationareamodel.h
STYLE_HEADERS += statusindicatormenu/notificationareastyle.h \
                 statusindicatormenu/statusindicatormenustyle.h

headers.path = /usr/include/system-ui
headers.files = statusindicatormenu/mstatusindicatormenuextensioninterface.h statusindicatormenu/MStatusIndicatorMenuExtensionInterface \
                statusindicatormenu/mstatusindicatormenuinterface.h
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = statusindicatormenu/system-ui.prf

INSTALLS += headers prf
