#ifndef ROAMINGCONTAINER_H
#define ROAMINGCONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiButtonGroup;
class DuiLinearLayoutPolicy;

class RoamingContainer : public DuiContainer
{
    Q_OBJECT

public:
    enum RoamingState {
        AlwaysAsk = 0,
        AlwaysAllow
    };

public:
    RoamingContainer(RoamingContainer::RoamingState state, bool roamingUpdatesState, DuiWidget *parent = 0);
    virtual ~RoamingContainer();
    void changeRoamingState(RoamingContainer::RoamingState state);

public slots:
    void toggleRoamingUpdates(bool toggle);

private slots:
    void buttonClicked(DuiButton *button);

signals:
    void roamingStateChanged(RoamingContainer::RoamingState);
    void roamingUpdatesToggled(bool);

private:
    void setLayout(RoamingContainer::RoamingState state, bool roamingUpdatesState);
    DuiButton* createButton(const QString &text, const QString &name, bool checked);
    void toggleLowerWidget(bool toggle);

private:
    QGraphicsWidget *lowerWidget;
    DuiButton *alwaysAskButton;
    DuiButton *alwaysAllowButton;
    DuiButton *updatesToggleButton;
    DuiButtonGroup *buttons;
    DuiLinearLayoutPolicy *mainLayoutPolicy;

};

#endif // ROAMINGCONTAINER_H
