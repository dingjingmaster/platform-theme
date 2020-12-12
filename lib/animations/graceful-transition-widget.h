#ifndef GRACEFUL_TRANSITION_WIDGET_H
#define GRACEFUL_TRANSITION_WIDGET_H
#include <cmath>
#include <QWidget>

#include "graceful.h"
#include "graceful-export.h"
#include "graceful-animation.h"

namespace Graceful
{
//* temporary widget used to perform smooth transition between one widget state and another
class GRACEFUL_EXPORT TransitionWidget : public QWidget
{
    Q_OBJECT

    //* declare opacity property
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    //* shortcut to painter
    typedef WeakPointer<TransitionWidget> Pointer;

    //* constructor
    TransitionWidget(QWidget *parent, int duration);

    //* destructor
    virtual ~TransitionWidget() = default;

    //*@name flags
    //@{
    enum Flag { None = 0, GrabFromWindow = 1 << 0, Transparent = 1 << 1, PaintOnWidget = 1 << 2 };

    Q_DECLARE_FLAGS(Flags, Flag)

    void setFlags(Flags value)
    {
        _flags = value;
    }

    void setFlag(Flag flag, bool value = true)
    {
        if (value) {
            _flags |= flag;
        } else {
            _flags &= (~flag);
        }
    }

    bool testFlag(Flag flag) const
    {
        return _flags.testFlag(flag);
    }

    //@}

    //* duration
    void setDuration(int duration)
    {
        if (_animation) {
            _animation.data()->setDuration(duration);
        }
    }

    //* duration
    int duration() const
    {
        return (_animation) ? _animation.data()->duration() : 0;
    }

    //* steps
    static void setSteps(int value)
    {
        _steps = value;
    }

    //*@name opacity
    //@{

    virtual qreal opacity() const
    {
        return _opacity;
    }

    virtual void setOpacity(qreal value)
    {
        value = digitize(value);
        if (_opacity == value) {
            return;
        }

        _opacity = value;
        update();
    }

    //@}

    //@name pixmaps handling
    //@{

    //* start
    void resetStartPixmap()
    {
        setStartPixmap(QPixmap());
    }

    //* start
    void setStartPixmap(QPixmap pixmap)
    {
        _startPixmap = pixmap;
    }

    //* start
    const QPixmap &startPixmap() const
    {
        return _startPixmap;
    }

    //* end
    void resetEndPixmap()
    {
        setEndPixmap(QPixmap());
    }

    //* end
    void setEndPixmap(QPixmap pixmap)
    {
        _endPixmap = pixmap;
        _currentPixmap = pixmap;
    }

    //* start
    const QPixmap &endPixmap() const
    {
        return _endPixmap;
    }

    //* current
    const QPixmap &currentPixmap() const
    {
        return _currentPixmap;
    }

    //@}

    //* grap pixmap
    QPixmap grab(QWidget * = 0, QRect = QRect());

    //* true if animated
    virtual bool isAnimated() const
    {
        return _animation.data()->isRunning();
    }

    //* end animation
    virtual void endAnimation()
    {
        if (_animation.data()->isRunning()) {
            _animation.data()->stop();
        }
    }

    //* animate transition
    virtual void animate()
    {
        if (_animation.data()->isRunning()) {
            _animation.data()->stop();
        }
        _animation.data()->start();
    }

    //* true if paint is enabled
    static bool paintEnabled();

protected:
    //* generic event filter
    virtual bool event(QEvent *);

    //* paint event
    virtual void paintEvent(QPaintEvent *);

    //* grab widget background
    /*!
    Background is not rendered properly using QWidget::render.
    Use home-made grabber instead. This is directly inspired from bespin.
    Copyright (C) 2007 Thomas Luebking <thomas.luebking@web.de>
    */
    virtual void grabBackground(QPixmap &, QWidget *, QRect &) const;

    //* grab widget
    virtual void grabWidget(QPixmap &, QWidget *, QRect &) const;

    //* fade pixmap
    virtual void fade(const QPixmap &source, QPixmap &target, qreal opacity, const QRect &) const;

    //* apply step
    virtual qreal digitize(const qreal &value) const
    {
        if (_steps > 0) {
            return std::floor(value * _steps) / _steps;
        } else {
            return value;
        }
    }

private:
    //* Flags
    Flags _flags = None;

    //* paint enabled
    static bool _paintEnabled;

    //* internal transition animation
    Animation::Pointer _animation;

    //* animation starting pixmap
    QPixmap _startPixmap;

    //* animation starting pixmap
    QPixmap _localStartPixmap;

    //* animation starting pixmap
    QPixmap _endPixmap;

    //* current pixmap
    QPixmap _currentPixmap;

    //* current state opacity
    qreal _opacity = 0;

    //* steps
    static int _steps;
};

}
#endif
