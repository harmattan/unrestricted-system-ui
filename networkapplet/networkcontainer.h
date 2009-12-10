#ifndef NETWORKCONTAINER_H
#define NETWORKCONTAINER_H

#include <DuiContainer>

class QGraphicsLinearLayout;
class DuiComboBox;
class DuiGridLayoutPolicy;
class DuiLabel;
class DuiLinearLayoutPolicy;
class NetworkList;

class NetworkContainer : public DuiContainer
{
    Q_OBJECT

public:
    NetworkContainer(DuiWidget *parent = 0);
    virtual ~NetworkContainer();

    void initModeComboBox(const QString &value, const QStringList &values);
    void initSelectionComboBox(const QString &value, const QStringList &values);
    void setDefaultSelection(const QString &value);    

public slots:        
    void showAvailableOperators(int selected, const QStringList &operators);
    void hideAvailableOperators();

signals:
    void networkModeChanged(QString);
    void networkSelectionChanged(QString);
    void availableOperatorSelected(int);

private:    
    void setLayout();
    QGraphicsWidget* comboBoxWidget(DuiLabel *label, DuiComboBox *comboBox);
    void initComboBox(DuiComboBox *cb, const QString &value, const QStringList &values);
    void toggleOperatorWidget(bool toggle);

private:
    DuiComboBox *modeComboBox;
    DuiComboBox *selComboBox;
    DuiLinearLayoutPolicy *layoutPolicy;
    NetworkList *networkList;
    QGraphicsWidget *operatorWidget;    
    DuiLabel *infoLabel;
    QGraphicsLinearLayout *mainLayout;

};

#endif // NETWORKCONTAINER_H
