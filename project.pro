# ####################################################################
# Main projectfile
# ####################################################################


TEMPLATE = subdirs
SUBDIRS = src batteryapplet batteryplugin displayapplet networkapplet connectivityplugin profileapplet profileplugin callandsimapplet tests

QMAKE_CLEAN += configure-stamp build-stamp \
