#ifndef NETWORKCONTAINER_H
#define NETWORKCONTAINER_H

#include <DuiContainer>

class DuiComboBox;
class DuiGridLayoutPolicy;
class DuiLabel;
class DuiLinearLayoutPolicy;
class NetworkList;

class NetworkContainer : public DuiContainer
{
    Q_OBJECT

public:
    NetworkContainer(DuiWidget *parent);
    virtual ~NetworkContainer();

    void setDefaultSelection();

public slots:
    void initModeComboBox(int selected, const QStringList &values);
    void initSelectionComboBox(int defaultIndex, int selected, const QStringList &values);
    void toggleAvailableNetworks(int selected, const QStringList &networks, bool toggle);

private slots:


signals:
    void networkModeChanged(QString);
    void networkSelectionChanged(QString);
    void availableNetworkSelected(QString);

private: //methods
    void setLayout();    
    void toggleComboBoxSignalConnection(DuiComboBox *cb, bool toggle = true);
    void initComboBox(DuiComboBox *cb, int selected, const QStringList &values);

private: //attributes
    DuiComboBox *modeComboBox;
    DuiComboBox *selectionComboBox;
    DuiLabel *infoLabel;
    DuiLabel *modeLabel;
    DuiLabel *selectionLabel;
    DuiLinearLayoutPolicy *layoutPolicy;
    int selectionDefaultIndex;
    NetworkList *networkList;

};

#endif // NETWORKCONTAINER_H
