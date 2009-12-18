# ####################################################################
# Main projectfile
# ####################################################################


TEMPLATE = subdirs
SUBDIRS = src batteryapplet batteryplugin displayapplet profileapplet profileplugin tests

QMAKE_CLEAN += configure-stamp build-stamp \
