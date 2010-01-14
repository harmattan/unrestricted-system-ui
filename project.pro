# ####################################################################
# System-UI main projectfile
# ####################################################################

TEMPLATE = subdirs
SUBDIRS = src \
          batteryapplet \
          batteryplugin \ 
          displayapplet \
          profileapplet \
          profileplugin \
          tests \
          translations

QMAKE_CLEAN += configure-stamp build-stamp
