#ifndef PROFILEBUTTONS_H
#define PROFILEBUTTONS_H

#include <DuiContainer>
#include <QHash>
class DuiButton;

class ProfileButtons : public DuiContainer
{
    Q_OBJECT

public:
    ProfileButtons(DuiWidget *parent=0);
    virtual ~ProfileButtons();

    void init(QMap<int, QString> data);
    int selectedProfile();
    QString selectedProfileName();

public slots:
    // returns false if selection won't change
    bool selectProfile(int id);

signals:
    void profileSelected(int id);

private slots:
    void buttonToggled(bool checked);

private:
    void createProfileButtons();
    void addButton(QString name, int id);
    void setLayout();
    void toggleSelected();

private:
    int selected;
    QHash<int, DuiButton*> buttons;
};

#endif // PROFILEBUTTONS_H
