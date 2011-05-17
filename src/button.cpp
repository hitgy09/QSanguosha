#include "button.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRotation>
#include <QPropertyAnimation>

#ifdef AUDIO_SUPPORT

#include "irrKlang.h"
extern irrklang::ISoundEngine *SoundEngine;

static QRectF ButtonRect(0, 0, 189, 46);

#endif

Button::Button(const QString &label, qreal scale)
    :label(label), size(ButtonRect.size() * scale),
    mute(true), font(Config.SmallFont)
{

    init();
}

Button::Button(const QString &label, const QSizeF &size)
    :label(label), size(size), mute(true), font(Config.SmallFont)
{
    init();
}

void Button::init()
{
    setFlags(ItemIsFocusable);

    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void Button::setMute(bool mute){
    this->mute = mute;
}

void Button::setFont(const QFont &font){
    this->font = font;
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *){
    setFocus(Qt::MouseFocusReason);

#ifdef AUDIO_SUPPORT
    if(SoundEngine && !mute)
        SoundEngine->play2D("audio/system/button-hover.ogg");
#endif
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event){
    event->accept();
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
#ifdef AUDIO_SUPPORT
    if(SoundEngine && !mute)
        SoundEngine->play2D("audio/system/button-down.ogg");
#endif

    emit clicked();
}

QRectF Button::boundingRect() const{
    return QRectF(QPointF(), size);
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF rect = boundingRect();

    QPainterPath path;
    path.addRoundedRect(rect, 5, 5);

    QColor rect_color(Qt::black);
    if(hasFocus())
        rect_color = QColor("orange");

    rect_color.setAlpha(0.43 * 255);
    painter->fillPath(path, rect_color);

    QPen pen(Config.TextEditColor);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawPath(path);

    painter->setFont(font);
    painter->setPen(Config.TextEditColor);
    painter->drawText(rect, Qt::AlignCenter, label);
}
