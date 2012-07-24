#include "safemodecontroller.h"
#include "safemodeplugin.h"
#include "../statusindicatormenu/statusindicatormenuverticalview.h"

SafeModeController::SafeModeController(SafeModePlugin *safeModePlugin, QGraphicsItem *parent) :
    CustomListItem(parent)
{
    Q_UNUSED(safeModePlugin);
    setStyleName("CommonBasicListItemInverted");
    setTitle(tr("Safe mode"));
    setSubtitle(tr("Click here to exit safe mode"));
    setImage("icon-l-error", QSize(64, 64));

    connect(this, SIGNAL(clicked()), this, SLOT(onItemClicked()));
}

void SafeModeController::onItemClicked()
{
    MMessageBox *box = new MMessageBox(tr("Exit safe mode"), tr("A faulty plugin caused system-ui to crash, are you sure you want to restart with all plugins enabled?"),
                                       M::YesButton | M::NoButton | M::HelpButton);

    int result = box->exec();
    box->deleteLater();
    if (result == QMessageBox::Yes) {
        QFile file(CRASH_FILE);

        if (file.exists())
            file.remove();

        // Ugly, but don't know any other way :/
        system("killall sysuid");
    } else if (result == QMessageBox::Help) {
        QDesktopServices::openUrl(QUrl("http://mohammadag.xceleo.org/system-ui-safe-mode-help.html"));
    }
}
