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
