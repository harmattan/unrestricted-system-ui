#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include <DuiWidget>

class DuiContainer;
class DuiLinearLayoutPolicy;
class DuiStatusIndicatorMenuInterface;
class OfflineContainer;
class PhoneNetworkContainer;

class Connectivity : public DuiWidget
{
    Q_OBJECT

public:
    Connectivity(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent = NULL);
    virtual ~Connectivity();

private slots:
    void showNetworkModificationPage();
    void toggleOfflineMode(bool toggle);    

private:
    void toggleContainersVisibility(bool toggle);
    // TEMP
    DuiContainer* createDummyContainer(const QString &text);

private:
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu; //! Interface for controlling the status indicator menu
    PhoneNetworkContainer *networkContainer;
    OfflineContainer *offlineContainer;
    DuiContainer *bluetoothContainer;
    DuiLinearLayoutPolicy *layoutPolicy;
};

#endif // CONNECTIVITY_H
