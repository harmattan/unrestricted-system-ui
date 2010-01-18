#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <DuiWidget>

class DuiApplicationPage;
class DuiStatusIndicatorMenuInterface;
class ProfileDataInterface;
class ProfileButtons;

/*!
 * The Profile widget makes it possible to select the currently
 * active profile.
 */
class ProfileWidget : public DuiWidget
{
    Q_OBJECT

public:
    ProfileWidget (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
                   QGraphicsItem *parent = NULL);
    virtual ~ProfileWidget ();
    /*!
     * \brief A method for retranslating the UI
     */
    void retranslateUi ();

private slots:
    /*!
     * \brief A slot for showing the profile duicontrolpanel plugin
     */
    void showProfileModificationPage ();

    /*!
     * \brief A slot for receiving information about profile button clicks.
     */
    void buttonClicked (int index);

    /*!
     * \brief A slot for receiving information about profile changes
     */
    void changeProfile (const QString &profileName);
    /*!
     * \brief A slot for reloading the translations on locale-change
     */
    void loadTranslation ();

private:
    /*!
     * \brief A method for initializing the profile buttons container
     */
    void initProfileButtons ();

private:
    //! Interface for controlling the status indicator menu
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu;

    //! Profile data If to receive information about possible profiles
    ProfileDataInterface            *dataIf;

    //! Profile buttongroup container
    ProfileButtons                  *profileButtons;

};

#endif // PROFILEWIDGET_H

