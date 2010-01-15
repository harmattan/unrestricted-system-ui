# ####################################################################
# System-UI main projectfile
# ####################################################################

TEMPLATE = subdirs
SUBDIRS = components \
          tests \
          translations

QMAKE_CLEAN += configure-stamp build-stamp
