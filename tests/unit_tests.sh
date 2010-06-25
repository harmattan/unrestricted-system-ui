#! /bin/bash

#
# If a unit test fails we the calculation of the coverage will fail. This script
# will print the names of those unit tests that are not set to be deleted and
# disabled. 
#
for f in ut_*; do
	if [ $f == "ut_template" ]; then
		continue
	fi

	if [ ! -e $f/disabled -a ! -e $f/deleted ]; then
		echo $f
	fi
done
