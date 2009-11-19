#ifndef BATTERY_H
#define BATTERY_H

#include <DuiWidget>

class DuiLabel;
class DuiStatusIndicatorMenuInterface;
class BatteryDBusInterface;
class BatteryImage;

class Battery : public DuiWidget
{
    Q_OBJECT

public:
    Battery(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent = NULL);
    virtual ~Battery();

private slots:
    void updateModeLabel(bool toggle);
    void updateTimeLabel(const QStringList &times);    

private: //methods
    QString timeValue(int minutes);

private: //attributes
    BatteryDBusInterface *dbusIf;
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu; //! Interface for controlling the status indicator menu
    DuiLabel *modeLabel;
    DuiLabel *timeLabel;
    BatteryImage *batteryImage;
};

#endif // BATTERY_H
