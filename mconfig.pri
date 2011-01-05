# Load more defines from the m_defines...
load(meegotouch_defines)
DEFINES += THEMEDIR=\\\"\"$$M_THEME_DIR\"\\\"

# For profiling with gprof:
#QMAKE_CXXFLAGS += -pg
#QMAKE_LFLAGS += -pg

QMAKE_CXXFLAGS += -g
QMAKE_LFLAGS += -g

# Directories
unix {
    SYSTEMUI_THEMES_DIR = /usr/share/themes/base/meegotouch/sysuid
}
mac {
     # Do mac stuff here
}
win32 {
     # Do win32 stuff here
}
