# Copies the style sheets, images and applet in shared directories
#
# Run in scratchbox, from directory where located.
#
#!/bin/sh

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0 [OPTIONS]"
    echo "Copies the system-ui style sheets, images and applet in shared directories"
    echo "Run in scratchbox."
    echo ""
    echo "  -h, --help    This help"
    echo "  -d, --delete  Deletes the copied files"
    echo ""
    exit;
elif [ "$1" = "-d" ] || [ "$1" = "--delete" ]; then
    rm /usr/lib/duicontrolpanel/battery.desktop > /dev/null 2>&1
    rm /usr/share/duicontrolpanel/themes/style/batteryapplet.css > /dev/null 2>&1
    rm /usr/lib/duicontrolpanel/applets/libbatteryapplet.so > /dev/null 2>&1
    echo "Battery applet files deleted!"
    exit;
elif [ "$1" != "" ]; then
    echo "No such option!"
    exit;
fi

## Make applet dirs
mkdir -p /usr/lib/duicontrolpanel/
mkdir -p /usr/share/duicontrolpanel/themes/style/
mkdir -p /usr/lib/duicontrolpanel/applets/

# Copy battery applet files
cp batteryapplet/battery.desktop /usr/lib/duicontrolpanel/
cp batteryapplet/batteryapplet.css /usr/share/duicontrolpanel/themes/style/
cp batteryapplet/lib/libbatteryapplet.so /usr/lib/duicontrolpanel/applets/
echo "Battery applet files exported!"

