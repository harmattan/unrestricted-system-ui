#ifndef PROFILECONTAINER_H
#define PROFILECONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiSlider;

class ProfileContainer: public DuiContainer
{
    Q_OBJECT

public:
    ProfileContainer(int id, const QString &title, DuiWidget *parent);
    virtual ~ProfileContainer();

    void initSlider(const QStringList& volumeLevels);
    void setLevel(int levelIndex);
    void setVibration(bool enabled);
    int id();

signals:
    void sliderValueChanged(int newValue);
    void vibrationChanged(bool on);

private:
    void setLayout();

private:
    QStringList volumeLevels;
    DuiSlider* slider;
    DuiButton* button;
    int levelIndex;
    const int profileId;
};

#endif // PROFILECONTAINER_H
