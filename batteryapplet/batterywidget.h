#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "dcpwidget.h" 

class DuiLabel;
class DuiButton;
class DuiSlider;
class DuiImage;
class QTimer;
class BatteryDBusInterface;

class BatteryWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    BatteryWidget(QGraphicsWidget *parent = 0);
    virtual ~BatteryWidget();

protected:
    void initWidget();

private slots:
    void sliderValueChanged(int value);
    void buttonPressed();
    void updateNotChargingImage(int level);
    void updateImage(bool charging = true, int level = -1);
    void updatePSMToggleButton(bool toggle);
    void updatePSMDisableButton(bool toggle);
    void updateButton(bool toggle, DuiButton *button);
    void batteryLevelValueChanged(int value);
    void startUpdatingChargingImage();
    void stopUpdatingChargingImage();
    void updateLabels(const QStringList &timeValues);
    void initSlider(const QStringList &values);
    void updateSlider(const QString &text);

private: //methods    
    void initImage();
    QString minutesInString(int mins, QString pattern);
    void togglePSMWidgets(bool enable);        

private: //attributes
    DuiLabel *talkTimeLabel;
    DuiLabel *standByTimeLabel;    
    DuiButton *PSMButton;
    DuiButton *PSMDisableButton;
    DuiSlider *slider;
    DuiImage *image;    
    QStringList batteryImages;
    QStringList batteryChargingImages;
    QStringList sliderValues;
    QTimer *updateChargingImageTimer;
    BatteryDBusInterface *batteryIf;

};
#endif // BATTERYWIDGET_H
