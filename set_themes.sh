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
    rm /usr/lib/duicontrolpanel/battery.desktop > /dev/null 2>&1
    rm /usr/share/duicontrolpanel/themes/style/batteryapplet.css > /dev/null 2>&1
    rm /usr/lib/duicontrolpanel/applets/libbatteryapplet.so > /dev/null 2>&1

    rm /usr/lib/duicontrolpanel/display.desktop > /dev/null 2>&1
    rm /usr/share/duicontrolpanel/themes/style/displayapplet.css > /dev/null 2>&1
    rm /usr/lib/duicontrolpanel/applets/libdisplayapplet.so > /dev/null 2>&1

    rm /usr/lib/duicontrolpanel/network.desktop > /dev/null 2>&1
    rm /usr/share/duicontrolpanel/themes/style/networkapplet.css > /dev/null 2>&1
    rm /usr/lib/duicontrolpanel/applets/libnetworkapplet.so > /dev/null 2>&1
    echo "Files deleted!"
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

## Make applet dirs
mkdir -p /usr/lib/duicontrolpanel/
mkdir -p /usr/share/duicontrolpanel/themes/style/
mkdir -p /usr/lib/duicontrolpanel/applets/

# Copy battery applet files
cp batteryapplet/battery.desktop /usr/lib/duicontrolpanel/
cp batteryapplet/batteryapplet.css /usr/share/duicontrolpanel/themes/style/
cp batteryapplet/lib/libbatteryapplet.so /usr/lib/duicontrolpanel/applets/

# Copy display applet files
cp displayapplet/display.desktop /usr/lib/duicontrolpanel/
cp displayapplet/displayapplet.css /usr/share/duicontrolpanel/themes/style/
cp displayapplet/lib/libdisplayapplet.so /usr/lib/duicontrolpanel/applets/

# Copy network applet files
cp networkapplet/network.desktop /usr/lib/duicontrolpanel/
cp networkapplet/networkapplet.css /usr/share/duicontrolpanel/themes/style/
cp networkapplet/lib/libnetworkapplet.so /usr/lib/duicontrolpanel/applets/

# Copy network applet files
cp profileapplet/profile.desktop /usr/lib/duicontrolpanel/
cp profileapplet/profileapplet.css /usr/share/duicontrolpanel/themes/style/
cp profileapplet/lib/libprofileapplet.so /usr/lib/duicontrolpanel/applets/
echo "Files copied!"
