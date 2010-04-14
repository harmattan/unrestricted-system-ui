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

DEFINES += STATUSINDICATORMENU_PLUGIN_DIR=\'$$quote(\"$$STATUSINDICATORMENU_PLUGIN_DIR\")\'

headers.path = /usr/include/duistatusindicatormenu
headers.files = statusindicatormenu/duistatusindicatormenuplugininterface.h statusindicatormenu/DuiStatusIndicatorMenuPluginInterface
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = statusindicatormenu/duistatusindicatormenu.prf

INSTALLS += headers prf
