#!/bin/bash

BCOVPATH=$(which bcov)

if [ "${BCOVPATH}x" == "x" ]; then
    echo "fakeroot apt-get install bcov"
    exit 1
fi

export COV_OPTION=on
export SBOX_USE_CCACHE=no

if [ -f Makefile ]; then
    make clean > /dev/null
    make distclean > /dev/null
fi

qmake COV_OPTION=on tests.pro
make -j4

echo "command make check" > .bcovdump.all
echo "date "`date` >> .bcovdump.all

for T in `ls -d ut_*`; do
	TEST=`basename $T`
    if [ -x $T/$TEST ]; then
	# Run the built tests
	echo -ne "Running $T/$TEST ..."
        CK_FORK="no" bcov $T/$TEST
	echo " done"
        if [ -f .bcovdump ]; then
            tail -n +5 .bcovdump >>.bcovdump.all
            rm -f .bcovdump
        fi
    fi
done

mkdir coverage &> /dev/null || rm -f coverage/* &> /dev/null
bcov-report .bcovdump.all ./coverage

echo "report available in coverage/index.html"

