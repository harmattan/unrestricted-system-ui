#include "shutdownui.h"

#include <DuiLabel>
#include <DuiSceneManager>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiLocale>

#include <QDebug>

ShutdownUI::ShutdownUI() :
        text(0)
{
    setComponentsDisplayMode(DuiApplicationPage::EscapeButton, DuiApplicationPageModel::Hide);
    setComponentsDisplayMode(DuiApplicationPage::NavigationBar, DuiApplicationPageModel::Hide);
    setComponentsDisplayMode(DuiApplicationPage::HomeButton, DuiApplicationPageModel::Hide);
    setPannableAreaInteractive(false);
}

ShutdownUI::~ShutdownUI()
{
}

void ShutdownUI::createContent()
{
    qDebug() << "ShutdownUI::createContent()";
    DuiApplicationPage::createContent();

    DuiWidget* panel = centralWidget();

    //% "Shutting down, good bye!"
    text = new DuiLabel (qtTrId ("qtn_shut_down"), panel);
    text->setAlignment(Qt::AlignCenter);

    alignText();
}

void ShutdownUI::showWindow()
{
    DuiApplicationWindow *win = DuiApplication::activeApplicationWindow();
    if (win && win->isHidden()) {
        win->show();
    }
    qDebug() << Q_FUNC_INFO << "win->isHidden()" << (win ? win->isHidden() : true);
}

void ShutdownUI::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    qDebug() << "ShutdownUI::resizeEvent()";
    DuiApplicationPage::resizeEvent(event);

    alignText();
}

void ShutdownUI::alignText()
{
    qDebug() << "ShutdownUI::alignText()";
    QSize size = DuiApplication::activeApplicationWindow()->sceneManager()->visibleSceneSize();
    text->setGeometry(QRectF(0, 0, size.width(), size.height()));
}
