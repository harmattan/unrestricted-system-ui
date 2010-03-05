
include(../../duiconfig.pri)

INCLUDEPATH += statusindicatormenu

SOURCES += statusindicatormenu/statusindicatormenuwindow.cpp \
    statusindicatormenu/pluginlist.cpp

HEADERS += statusindicatormenu/statusindicatormenuwindow.h \
    statusindicatormenu/pluginlist.h

DEFINES += DUISTATUSINDICATORMENU_PLUGIN_DIR=\'$$quote(\"$$DUISTATUSINDICATORMENU_PLUGIN_DIR\")\'

headers.path = /usr/include/duistatusindicatormenu
headers.files = statusindicatormenu/duistatusindicatormenuplugininterface.h statusindicatormenu/DuiStatusIndicatorMenuPluginInterface
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = statusindicatormenu/duistatusindicatormenu.prf

INSTALLS += headers prf
