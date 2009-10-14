# Copies the style sheets, images and applets in shared directories
#
# Run in scratchbox, from directory where located
#
#!/bin/sh

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0 [OPTIONS]"
    echo "Copies the system-ui style sheets, images and applet in shared directories"
    echo "Run in scratchbox, from directory har-systemui/cellular-ui/trunk/"
    echo ""
    echo "  -h, --help    This help"
    echo "  -d, --delete  Deletes the copied files"
    echo ""
    exit;
elif [ "$1" = "-d" ] || [ "$1" = "--delete" ]; then
    rm /usr/lib/duicontrolpanel/profile.desktop > /dev/null 2>&1
    rm /usr/share/duicontrolpanel/themes/style/profilepplet.css > /dev/null 2>&1
    rm /usr/lib/duicontrolpanel/applets/libprofileapplet.so > /dev/null 2>&1
    echo "Profile applet files deleted!"
    exit;
elif [ "$1" != "" ]; then
    echo "No such option!"
    exit;
fi

## Make applet dirs
mkdir -p /usr/lib/duicontrolpanel/
mkdir -p /usr/share/duicontrolpanel/themes/style/
mkdir -p /usr/lib/duicontrolpanel/applets/

# Copy profile applet files
cp profileapplet/profile.desktop /usr/lib/duicontrolpanel/
cp profileapplet/profileapplet.css /usr/share/duicontrolpanel/themes/style/
cp profileapplet/lib/libprofileapplet.so /usr/lib/duicontrolpanel/applets/
echo "Profile applet files exported!"

