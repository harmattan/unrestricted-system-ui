# Copies the style cheets and images in shared directories
#
# Run in scratchbox, from directory har-systemui/cellular-ui/trunk/
#
mkdir -p /usr/share/sysuid/themes/style/
mkdir -p /usr/share/sysuid/themes/svg/
mkdir -p /usr/share/sysuid/themes/images/
cp data/*.svg /usr/share/sysuid/themes/svg/
cp src/*.css /usr/share/sysuid/themes/style/
cp src/*.svg /usr/share/sysuid/themes/svg/
cp src/unlocksliderwidget/*.css /usr/share/sysuid/themes/style/
cp src/unlocksliderwidget/*.svg /usr/share/sysuid/themes/svg/


# Copies the applets
#
cp batteryapplet/battery.desktop /usr/lib/duicontrolpanel/
cp batteryapplet/*.css /usr/share/duicontrolpanel/themes/style/
cp batteryapplet/lib/libbatteryapplet.so /usr/lib/duicontrolpanel/applets/
cp displayapplet/display.desktop /usr/lib/duicontrolpanel/
cp displayapplet/*.css /usr/share/duicontrolpanel/themes/style/
cp displayapplet/lib/*.so /usr/lib/duicontrolpanel/applets/
cp networkapplet/*.desktop /usr/lib/duicontrolpanel/
cp networkapplet/*.css /usr/share/duicontrolpanel/themes/style/
cp networkapplet/lib/*.so /usr/lib/duicontrolpanel/applets/

