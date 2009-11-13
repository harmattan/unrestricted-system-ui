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

    void initModeComboBox(const QString &value, const QStringList &values);
    void initSelectionComboBox(const QString &value, const QStringList &values);
    void setDefaultSelection(const QString &value);
    bool operatorSelected();

public slots:        
    void toggleAvailableOperators(int selected, const QStringList &operators, bool toggle);

signals:
    void networkModeChanged(QString);
    void networkSelectionChanged(QString);
    void availableOperatorSelected(int);

private: //methods
    void setLayout();    
    void toggleComboBoxSignalConnection(DuiComboBox *cb, bool toggle = true);
    void initComboBox(DuiComboBox *cb, const QString &value, const QStringList &values);

private: //attributes
    DuiComboBox *modeComboBox;
    DuiComboBox *selectionComboBox;
    DuiLabel *infoLabel;
    DuiLabel *modeLabel;
    DuiLabel *selectionLabel;
    DuiLinearLayoutPolicy *layoutPolicy;    
    NetworkList *networkList;    

};

#endif // NETWORKCONTAINER_H
