# Load more defines from the m_defines...
load(meegotouch_defines)
# Silence the compiler
CONFIG += silent
# Directories
unix {
    DUISTATUSINDICATORMENU_PREFIX = /usr
    DUISTATUSINDICATORMENU_DIR = $$DUISTATUSINDICATORMENU_PREFIX/share/duistatusindicatormenu
    DUISTATUSINDICATORMENU_PLUGIN_DIR = $$DUISTATUSINDICATORMENU_PREFIX/lib/duistatusindicatormenu/plugins
     
    SYSTEMUI_THEMES_DIR = /usr/share/themes/base/meegotouch/sysuid
}
mac {
     # Do mac stuff here
}
win32 {
     # Do win32 stuff here
}
