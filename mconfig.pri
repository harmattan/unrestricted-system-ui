# Load more defines from the m_defines...
load(meegotouch_defines)
# Silence the compiler
CONFIG += silent
DEFINES += THEMEDIR=\\\"\"$$M_THEME_DIR\"\\\"

# For profiling with gprof:
#QMAKE_CXXFLAGS += -pg
#QMAKE_LFLAGS += -pg

# Directories
unix {
    STATUSINDICATORMENU_PLUGIN_DIR = /usr/lib/meegotouch/statusindicatormenuplugins
    SYSTEMUI_THEMES_DIR = /usr/share/themes/base/meegotouch/sysuid
}
mac {
     # Do mac stuff here
}
win32 {
     # Do win32 stuff here
}
