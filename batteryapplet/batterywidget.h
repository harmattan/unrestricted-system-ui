#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "dcpwidget.h" 
#include "batterygconf.h"

class DuiLabel;
class DuiButton;
class DuiSlider;
class DuiImage;
class QTimer;

class BatteryWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    BatteryWidget(QGraphicsWidget *parent = 0);
    virtual ~BatteryWidget();

protected:
    void initWidget();

private slots:
    void sliderValueChanged(int newValue);
    void buttonPressed();
    void gConfValueChanged(BatteryGConf::GConfKey key, QVariant value);
    void updateImage(bool charging = true, int level = -1);

private: //methods
    void initSlider();
    void initImage();
    QString minutesInString(int mins, QString pattern);
    void updateButton(DuiButton *button, bool toggle);
    void updateSlider(const QString &text);
    void updateLabel(DuiLabel *label, const QString &text);    
    void togglePSMWidgets(bool enable);

private: //attributes
    DuiLabel *talkTimeLabel;
    DuiLabel *standByTimeLabel;    
    DuiButton *PSMButton;
    DuiButton *disablePSMButton;   
    DuiSlider *slider;
    DuiImage *image;
    QList<QVariant> sliderValues;
    QStringList batteryImages;
    QStringList batteryChargingImages;
    QTimer *imageUpdateTimer;
    BatteryGConf *batteryGConf;    

};
#endif // BATTERYWIDGET_H
