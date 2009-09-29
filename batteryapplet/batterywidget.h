#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "dcpwidget.h" 

class BatteryDBusInterface;
class DuiLabel;
class DuiButton;
class DuiGridLayoutPolicy;
class DuiImage;
class DuiSlider;
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
    void sliderValueChanged(int value);
    void PSMButtonPressed();
    void PSMAutoButtonPressed();
    void updateNotChargingImage(int level);
    void updateImage(bool charging = true);
    void updatePSMButton(bool toggle);
    void updatePSMAutoButton(bool toggle);
    void batteryLevelValueChanged(int value);
    void startUpdatingChargingImage();
    void stopUpdatingChargingImage();
    void updateLabels(const QStringList &timeValues);
    void initSlider(const QStringList &values);
    void updateSlider(const QString &text);

private: //methods    
    void initImage();
    QString minutesInString(int mins, const QString &pattern);
    void toggleSliderVisibility(bool toggle);

private: //attributes
    int batteryLevel;
    BatteryDBusInterface *batteryIf;
    DuiLabel *talkTimeLabel;
    DuiLabel *standByTimeLabel;    
    DuiButton *PSMButton;
    DuiButton *PSMAutoButton;
    DuiGridLayoutPolicy *sliderLayoutPolicy;
    DuiImage *image;
    DuiSlider *slider;
    QStringList batteryImages;
    QStringList batteryChargingImages;
    QStringList sliderValues;
    QTimer *updateChargingImageTimer;
};
#endif // BATTERYWIDGET_H
