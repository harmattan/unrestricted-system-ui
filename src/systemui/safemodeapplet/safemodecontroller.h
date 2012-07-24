#ifndef SAFEMODECONTROLLER_H
#define SAFEMODECONTROLLER_H

#include <MContentItem>
#include <MLabel>
#include <MImageWidget>
#include <MListItem>
#include <MMessageBox>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QDesktopServices>
#include <QUrl>

class CustomListItem : public MListItem
{
public:
    CustomListItem(QGraphicsItem *parent = 0) : MListItem(parent) {
        setObjectName("CommonLargePanel");

        // Layout
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
        QGraphicsLinearLayout *textLayout = new QGraphicsLinearLayout(Qt::Vertical);

        // Title
        titleLabel = new MLabel(this);
        titleLabel->setTextElide(true);
        titleLabel->setObjectName("CommonTitleInverted");

        // Subtitle
        subtitleLabel = new MLabel(this);
        subtitleLabel->setTextElide(true);
        subtitleLabel->setWordWrap(true);
        subtitleLabel->setObjectName("CommonSubTitleInverted");

        // Icon
        image = new MImageWidget(this);
        image->setObjectName("CommonMainIcon");

        // Construct layout
        textLayout->addItem(titleLabel);
        textLayout->addItem(subtitleLabel);
        textLayout->setSpacing(0);

        layout->addItem(image);
        layout->addItem(textLayout);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(layout->spacing()-0.2);
    }

    void setTitle(const QString &title)
    {
        titleLabel->setText(title);
    }

    void setSubtitle(const QString &subtitle)
    {
        subtitleLabel->setText(subtitle);
    }

    void setImage(const QImage &picture)
    {
        image->setImage(picture);
    }

    void setImage(const QString &imageID, const QSize &size)
    {
        image->setImage(imageID, size);
    }

private:
    MLabel *titleLabel;
    MLabel *subtitleLabel;
    MImageWidget *image;
};

class SafeModePlugin;

class SafeModeController : public CustomListItem
{
    Q_OBJECT
public:
    explicit SafeModeController(SafeModePlugin *safeModePlugin, QGraphicsItem *parent = NULL);
    
private slots:
    void onItemClicked();
};

#endif // SAFEMODECONTROLLER_H
