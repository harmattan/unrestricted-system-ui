#include "notifier.h"

#include <DuiNotification>
#include <DuiLocale>
#include <QTimer>
#include <QStringList>


//TODO.: Use the logical names when available
namespace {
     const QStringList NotificationTexts = (QStringList()
                                            << "SIM card inserted."
                                            << "SIM card not inserted"
                                            << "SIM card removed."
                                            << "SIM card permanently blocked."
                                            << "SIM card rejected."
                                            << "Emergency call starting."
                                            << "Emergency call finished."
                                            << "PIN code correct."
                                            << "PUK code correct."
                                            << "Generic technical problem."
                                            << ""
                                            << "PIN code incorrect."
                                            << "PIN code incorrect. 2 attempts left."
                                            << "PIN code incorrect. 1 attempt left."
                                            << "PIN code incorrect. SIM locked with PUK code."
                                            << "PUK code incorrect."
                                            << "PUK code incorrect. SIM card permanently blocked."
                                            << "PIN code changed."
                                            << "PIN codes do not match.");
}

Notifier::Notifier() : QObject()
{
}

Notifier::~Notifier()
{
}

void Notifier::showNotification(Notification notification)
{
    DuiNotification* n = new DuiNotification(DuiNotification::Information,
                                             "Icon-close",
                                             QString( "<font color=\"white\">" + mapNotificationText(notification) + "</font>"),
                                             "");
    QTimer *t = new QTimer(n);
    connect(t, SIGNAL(timeout()), this, SLOT(notificationTimeout()));
    t->setSingleShot(true);
    t->start(3000);
    n->appear(DuiSceneWindow::DestroyWhenDone);
}

void Notifier::notificationTimeout()
{
    QTimer *t = qobject_cast<QTimer *>(sender());

    if (t != NULL) {
        DuiNotification *dn = qobject_cast<DuiNotification *>(t->parent());
        if (dn != NULL) {
            dn->disappear();
        }
    }
}

QString Notifier::mapNotificationText(Notification notification)
{
    QString notifText;
    switch(notification)
    {
    case SIMCardInserted:
        notifText = trid("NOT DOCUMENTED YET", "SIM card inserted.");
        break;
    case SIMCardNotInserted:
        notifText = trid("NOT DOCUMENTED YET" , "SIM card not inserted.");
        break;
    case SIMCardRemoved:
        notifText = trid("NOT DOCUMENTED YET" , "SIM card removed.");
        break;
    case SIMCardRejected:
        notifText = trid("qtn_cell_sim_rejected" , "SIM card rejected.");
        break;
    case SIMCardPermanentlyBlocked:
        notifText = trid("NOT DOCUMENTED YET" , "SIM card permanently blocked.");
        break;
    case EmergencyCallStarting:
        notifText = trid("qtn_cell_emergency_start" , "Starting emergency call.");
        break;
    case EmergencyCallEnded:
        notifText = trid("qtn_cell_emergency_end" , "Emergency call ended.");
        break;

    /* At the moment this contains several error cases that may pop up from SIM library.
       It needs to be clarified, which of those and how are they going to be reported
       to end user. */
    case TechnicalProblem:
        notifText = trid("NOT DOCUMENTED YET" , "Technical problem.");
        break;

    case Empty:
        notifText = trid("" , ""); //this is never shown
        break;
    case PINCodeIncorrect:
        notifText = trid("" , "");//this is never shown
        break;
    case PINCodeIncorrect2AttemptsLeft:
        notifText = trid("qtn_cell_incorrect_pin_2" , "Incorrect PIN code. 2 attempts remaining.");
        break;
    case PINCodeIncorrect1AttemptLeft:
        notifText = trid("qtn_cell_incorrect_pin_1" , "Incorrect PIN code. 1 attempt remaining.");
        break;
    case PINCodeIncorrectPUKCodeRequired:
        notifText = trid("qtn_cell_incorrect_pin_0" , "Incorrect PIN code. SIM locked with PUK code.");
        break;
    case PUKCodeIncorrect:
        notifText = trid("qtn_cell_incorrect_puk" , "Incorrect PUK code.");
        break;
    case PUKCodeIncorrectSIMCardPermanentlyBlocked:
        notifText = trid("qtn_cell_sim_rejected" , "SIM card rejected."); //Should this be different?
        break;
    case PINCodeChanged:
        notifText = trid("qtn_cell_pin_code_changed" , "PIN code changed.");
        break;
    case PINCodesDoNotMatch:
        notifText = trid("qtn_cell_codes_not_match" , "Codes do not match.");
        break;
    }
    return notifText;
}
