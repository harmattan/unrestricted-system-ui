
include(../../mconfig.pri)

INCLUDEPATH += statusindicatormenu

SOURCES += \
    statusindicatormenu/statusindicatormenuwindow.cpp \
    statusindicatormenu/pluginlist.cpp \
    statusindicatormenu/notificationarea.cpp \
    statusindicatormenu/notificationareaview.cpp

HEADERS += \
    statusindicatormenu/statusindicatormenuwindow.h \
    statusindicatormenu/pluginlist.h \
    statusindicatormenu/notificationarea.h \
    statusindicatormenu/notificationareamodel.h \
    statusindicatormenu/notificationareaview.h \
    statusindicatormenu/notificationareastyle.h

MODEL_HEADERS += statusindicatormenu/notificationareamodel.h
STYLE_HEADERS += statusindicatormenu/notificationareastyle.h

DEFINES += DUISTATUSINDICATORMENU_PLUGIN_DIR=\'$$quote(\"$$DUISTATUSINDICATORMENU_PLUGIN_DIR\")\'

headers.path = /usr/include/mstatusindicatormenu
headers.files = statusindicatormenu/mstatusindicatormenuplugininterface.h statusindicatormenu/DuiStatusIndicatorMenuPluginInterface
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = statusindicatormenu/mstatusindicatormenu.prf

INSTALLS += headers prf
