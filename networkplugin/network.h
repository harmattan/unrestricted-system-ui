#ifndef NETWORK_H
#define NETWORK_H

#include <DuiWidget>

class DuiButton;
class DuiImage;
class DuiLabel;
class DuiStatusIndicatorMenuInterface;
class NetworkBusinessLogic;


class Network : public DuiWidget
{
    Q_OBJECT

public:
    Network(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent = NULL);
    virtual ~Network();

private slots:
    void updateButton(bool toggle);
    void updateButtonImage(const QString &icon);
    void updateSignalStrengthImage(const QString &icon);

private: //methods
    void toggleWidgetsVisibility(bool toggle);

private: //attributes
    NetworkBusinessLogic *logic;
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu; //! Interface for controlling the status indicator menu    
    DuiButton *toggleButton;
    DuiImage *signalStrengthImage;
    DuiLabel *networkLabel;
    DuiLabel *operatorLabel;

};

#endif // NETWORK_H
