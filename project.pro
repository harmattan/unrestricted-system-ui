TEMPLATE = subdirs
SUBDIRS = src \
          tests \
          translations

QMAKE_CLEAN += configure-stamp build-stamp
