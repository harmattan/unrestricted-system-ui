include(../../mconfig.pri)

INCLUDEPATH += statusindicatormenu

SOURCES += \
    statusindicatormenu/statusindicatormenuwindow.cpp \
    statusindicatormenu/statusindicatormenuadaptor.cpp \
    statusindicatormenu/notificationarea.cpp \
    statusindicatormenu/notificationareaview.cpp

HEADERS += \
    statusindicatormenu/statusindicatormenuwindow.h \
    statusindicatormenu/statusindicatormenuadaptor.h \
    statusindicatormenu/notificationarea.h \
    statusindicatormenu/notificationareamodel.h \
    statusindicatormenu/notificationareaview.h \
    statusindicatormenu/notificationareastyle.h \
    statusindicatormenu/statusindicatormenuwindowstyle.h

MODEL_HEADERS += statusindicatormenu/notificationareamodel.h
STYLE_HEADERS += statusindicatormenu/notificationareastyle.h \
                 statusindicatormenu/statusindicatormenuwindowstyle.h

DEFINES += STATUSINDICATORMENU_PLUGIN_DIR=\'$$quote(\"$$STATUSINDICATORMENU_PLUGIN_DIR\")\'

headers.path = /usr/include/system-ui
headers.files = statusindicatormenu/mstatusindicatormenuplugininterface.h statusindicatormenu/MStatusIndicatorMenuPluginInterface \
                statusindicatormenu/mstatusindicatormenuextensioninterface.h statusindicatormenu/MStatusIndicatorMenuExtensionInterface \
                statusindicatormenu/mstatusindicatormenuinterface.h
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = statusindicatormenu/system-ui.prf

INSTALLS += headers prf
