#ifndef BATTERY_H
#define BATTERY_H

#include <DuiWidget>

class DuiLabel;
class DuiContainer;
class DuiStatusIndicatorMenuInterface;
class BatteryDBusInterface;
class BatteryImage;

class Battery : public DuiWidget
{
    Q_OBJECT

public:
    Battery (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
             QGraphicsItem *parent = NULL);
    virtual ~Battery ();
    void retranslateUi ();

private slots:
    void updateModeLabel (bool toggle);
    void updateTimeLabel (const QStringList &times);
    void showBatteryModificationPage ();
    void loadTranslation ();

private: //methods
    QString timeValue (int minutes);

private: //attributes
    BatteryDBusInterface            *dbusIf;
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu;
    //^^ ! Interface for controlling the status indicator menu
    DuiLabel                        *modeLabel;
    DuiLabel                        *timeLabel;
    BatteryImage                    *batteryImage;
    DuiContainer                    *container;
    bool                             PSMode;
    QStringList                     *last_values;
};

#endif // BATTERY_H
