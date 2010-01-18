#include "battery.h"
#include "batterydbusinterface.h"
#include "batteryimage.h"

#include <DuiApplicationIfProxy>
#include <DuiButton>
#include <DuiContainer>
#include <DuiControlPanelIf>
#include <DuiGridLayoutPolicy>
#include <DuiImage>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiTheme>

#include <QGraphicsLinearLayout>
#include <QDebug>

#define SYSTEMUI_TRANSLATION "duicontrolpanel-systemui"

const QString cssDir = "/usr/share/duistatusindicatormenu/themes/style/";

Battery::Battery (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
                 QGraphicsItem *parent) :
        DuiWidget (parent),
        dbusIf (NULL),
        statusIndicatorMenu (statusIndicatorMenu),
        modeLabel (NULL),
        timeLabel (NULL),
        batteryImage (NULL),
        container (NULL)
{
    DuiTheme::loadCSS(cssDir + "batteryplugin.css");

    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // init widgets
    modeLabel = new DuiLabel();
    modeLabel->setObjectName("batteryModeLabel");
    timeLabel = new DuiLabel();
    timeLabel->setObjectName("batteryTimeLabel");
    batteryImage = new BatteryImage();
    batteryImage->setObjectName("batteryImage");

    // Create a container for the battery
    container = new DuiContainer;
    DuiWidget *widget = new DuiWidget;
    DuiLayout *layout = new DuiLayout;
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    widget->setLayout(layout);
    //% "Battery"
    container->setTitle(qtTrId ("qtn_ener_battery"));
    container->setCentralWidget(widget);
    connect(container, SIGNAL(headerClicked()), this, SLOT(showBatteryModificationPage()));
    mainLayout->addItem(container);

    // insert widgets
    layoutPolicy->addItemAtPosition(modeLabel, 0, 0, 1, 1);
    layoutPolicy->addItemAtPosition(timeLabel, 0, 1, 1, 2);
    layoutPolicy->addItemAtPosition(batteryImage, 1, 0, 1, 2);

    // get widget values
    dbusIf = new BatteryDBusInterface();
    connect(dbusIf, SIGNAL(PSMValueReceived(bool)), this, SLOT(updateModeLabel(bool)));
    connect(dbusIf, SIGNAL(remainingTimeValuesReceived(QStringList)), this, SLOT(updateTimeLabel(QStringList)));
    connect(dbusIf, SIGNAL(batteryBarValueReceived(int)), batteryImage, SLOT(updateBatteryLevel(int)));
    connect(dbusIf, SIGNAL(batteryCharging(int)), batteryImage, SLOT(startCharging(int)));
    connect(dbusIf, SIGNAL(batteryNotCharging()), batteryImage, SLOT(stopCharging()));

    dbusIf->PSMValueRequired();
    dbusIf->remainingTimeValuesRequired();
    dbusIf->batteryBarValueRequired();
    dbusIf->batteryChargingStateRequired();

}

Battery::~Battery()
{
    delete dbusIf;
    dbusIf = NULL;
}

void Battery::updateModeLabel(bool toggle)
{
    modeLabel->setText ((toggle ?
                         //% "Power save mode"
                         qtTrId ("qtn_ener_psmode") :
                         //% "Normal mode"
                         qtTrId ("qtn_ener_normal"))
                       );
}

void Battery::updateTimeLabel(const QStringList &times)
{
    if (times.size() != 2)
        return;

    if (times.at(1).toInt() == 0) // a text, not minute value
        timeLabel->setText(times.at(1));
    else
        timeLabel->setText(timeValue(times.at(1).toInt()));
}

QString Battery::timeValue(int minutes)
{
    //% "%a hours %b minutes"
    const QString TimeValueText = qtTrId ("qtn_ener_remtime");
    const QString minutesPrefix = TimeValueText.section("%b", 1, 1).trimmed();
    const QString hoursPrefix = (TimeValueText.section("%b", 0, 0)).section("%a", 1, 1).trimmed();
    QString time;

    if (minutes < 60)
        time = QString("%1%2").arg(minutes).arg(minutesPrefix);
    else {
        QVariant minsVar = minutes % 60;
        minsVar = (minsVar.toInt() == 0) ? "00" : minsVar;
        time = QString("%1:%2%3").arg(minutes / 60).arg(minsVar.toString()).arg(hoursPrefix);
    }
    return time;
}

void Battery::showBatteryModificationPage()
{
    // instantiate the interface
    DuiControlPanelIf cpIf;
    // check the interface is valid
    if (!cpIf.isValid())
        return;
    cpIf.appletPage("Battery");
}

void Battery::retranslateUi ()
{
    DuiLocale   locale;

    locale.installTrCatalog (SYSTEMUI_TRANSLATION);

    // This call will reload modeLabel and timeLabel
    // contents with the actual translations:
    dbusIf->PSMValueRequired();
    dbusIf->remainingTimeValuesRequired();

    container->setTitle(qtTrId ("qtn_ener_battery"));
}

