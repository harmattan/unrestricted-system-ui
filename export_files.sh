# Copies the style sheets, images and applets in shared directories
#
# Run in scratchbox, from directory har-systemui/cellular-ui/trunk/
#
#!/bin/sh

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0 [OPTIONS]"
    echo "Copies the system-ui style sheets, images and applets in shared directories"
    echo "Run in scratchbox, from directory har-systemui/cellular-ui/trunk/"
    echo ""
    echo "  -h, --help    This help"
    echo "  -d, --delete  Deletes the copied files"
    echo ""
    exit;
elif [ "$1" = "-d" ] || [ "$1" = "--delete" ]; then
    rm -r /usr/share/sysuid/ > /dev/null 2>&1

	./batteryapplet/export_files.sh -d
	./displayapplet/export_files.sh -d
	./networkapplet/export_files.sh -d
	./profileapplet/export_files.sh -d

    echo "System-ui files deleted!"
    exit;
elif [ "$1" != "" ]; then
    echo "No such option!"
    exit;
fi

## Copy application themes
mkdir -p /usr/share/sysuid/themes/style/
mkdir -p /usr/share/sysuid/themes/svg/
mkdir -p /usr/share/sysuid/themes/images/
cp data/*.svg /usr/share/sysuid/themes/svg/
cp src/*.css /usr/share/sysuid/themes/style/
cp src/*.svg /usr/share/sysuid/themes/svg/
cp src/unlocksliderwidget/*.css /usr/share/sysuid/themes/style/
cp src/unlocksliderwidget/*.svg /usr/share/sysuid/themes/svg/

./batteryapplet/export_files.sh
./displayapplet/export_files.sh
./networkapplet/export_files.sh
./profileapplet/export_files.sh
./../../profile-qt/trunk/export_files.sh
echo "System-ui files exported!"


