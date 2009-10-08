#!/bin/sh
# Validates the test xml

if [ ! -f ./testdefinition.xsd ]; then
    echo "schema file not found. Try getting it (outside scratchbox):"
    echo " wget --user=pmouser --password=pmopassword https://projects.maemo.org/docs/_downloads/testdefinition.xsd"
else
    xmllint --noout --schema testdefinition.xsd tests.xml
fi

