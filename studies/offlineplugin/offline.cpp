#include "offline.h"

#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiTheme>
#include <DuiLocale>
#include <DuiContainer>
#include <DuiLabel>
#include <DuiButton>

#include <QGraphicsLinearLayout>

const QString cssDir("/usr/share/duistatusindicatormenu/themes/style/");

Offline::Offline(DuiStatusIndicatorMenuInterface& statusIndicatorMenu, QGraphicsItem* parent) :
        DuiWidget(parent),
        statusIndicatorMenu(statusIndicatorMenu),
        label(NULL),
        button(NULL)
{
    DuiTheme::loadCSS(cssDir + "offlineplugin.css");

    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);

    DuiWidget *widget = new DuiWidget;
    widget->setLayout(layout);

    DuiContainer *container = new DuiContainer;
    container->setCentralWidget(widget);
    mainLayout->addItem(container);

    label = new DuiLabel(trid("qtn_offline_offline", "Offline mode"));
    layout->addItem(label);
    layout->addStretch();

    button = new DuiButton;
    layout->addItem(button);
}

Offline::~Offline()
{
}
