#ifndef GRACEFUL_SCROLLBAR_ENGINE_H
#define GRACEFUL_SCROLLBAR_ENGINE_H

#include "graceful-export.h"
#include "graceful-scrollbar-data.h"
#include "graceful-widget-state-engine.h"

namespace Graceful
{
//* stores scrollbar hovered action and timeLine
class GRACEFUL_EXPORT ScrollBarEngine : public WidgetStateEngine
{
    Q_OBJECT

public:
    //* constructor
    explicit ScrollBarEngine(QObject *parent)
        : WidgetStateEngine(parent)
    {

    }

    //* destructor
    virtual ~ScrollBarEngine()
    {

    }

    //* register scrollbar
    virtual bool registerWidget(QWidget *, AnimationModes);

    //*@name accessors
    //@{

    using WidgetStateEngine::isAnimated;
    using WidgetStateEngine::opacity;

    //* true if widget is animated
    virtual bool isAnimated(const QObject *, AnimationMode, QStyle::SubControl control);

    //* true if widget is animated
    virtual AnimationMode animationMode(const QObject *object, QStyle::SubControl control);

    //* animation opacity
    virtual qreal opacity(const QObject *object, QStyle::SubControl control);

    //* return true if given subcontrol is hovered
    virtual bool isHovered(const QObject *object, QStyle::SubControl control)
    {

        if (DataMap<WidgetStateData>::Value data = this->data(object, AnimationHover)) {
            return static_cast<const ScrollBarData *>(data.data())->isHovered(control);
        } else {
            return false;
        }
    }

    //* control rect associated to object
    virtual QRect subControlRect(const QObject *object, QStyle::SubControl control)
    {

        if (DataMap<WidgetStateData>::Value data = this->data(object, AnimationHover)) {
            return static_cast<const ScrollBarData *>(data.data())->subControlRect(control);
        } else {
            return QRect();
        }
    }

    //* mouse position
    virtual QPoint position(const QObject *object)
    {

        if (DataMap<WidgetStateData>::Value data = this->data(object, AnimationHover)) {
            return static_cast<const ScrollBarData *>(data.data())->position();
        } else {
            return QPoint(-1, -1);
        }
    }

    //@}

    //*@name modifiers
    //@{

    //* control rect
    virtual void setSubControlRect(const QObject *object, QStyle::SubControl control, const QRect &rect)
    {

        if (DataMap<WidgetStateData>::Value data = this->data(object, AnimationHover)) {
            static_cast<ScrollBarData *>(data.data())->setSubControlRect(control, rect);
        }
    }

    //@}
};

}
#endif
