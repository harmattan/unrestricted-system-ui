#! /bin/sh

LCOV=`which lcov`

if test "x$LCOV" = x; then
    echo "You need to install lcov to get actual reports!";
    echo "See http://ltp.sf.net/coverage/lcov.php";
    echo "!!! ATTENTION !!!"
    echo "The current lcov-1.8 contains some silly bug.";
    echo "Please, use a newer one or the cvs version."
    exit 1;
fi

if test "x$SBOX_USE_CCACHE" = xyes; then
	export SBOX_USE_CCACHE=no
fi

rm -fr ./coverage/*

echo "qmake BUILD_FEATURES=coverage"
qmake BUILD_FEATURES=coverage

echo "make coverage"
make clean
make coverage

ls -lh ./coverage/index.html

