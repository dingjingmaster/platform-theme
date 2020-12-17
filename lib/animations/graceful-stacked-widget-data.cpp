#include "graceful-stacked-widget-data.h"

namespace Graceful
{

StackedWidgetData::StackedWidgetData(QObject *parent, QStackedWidget *target, int duration)
    : TransitionData(parent, target, duration)
    , _target(target)
    , _index(target->currentIndex())
{
    CT_SYSLOG(LOG_DEBUG, "");
    // configure transition
    connect(_target.data(), SIGNAL(destroyed()), SLOT(targetDestroyed()));
    connect(_target.data(), SIGNAL(currentChanged(int)), SLOT(animate()));

    // disable focus
    transition().data()->setAttribute(Qt::WA_NoMousePropagation, true);
    transition().data()->setFlag(TransitionWidget::PaintOnWidget, true);

    setMaxRenderTime(50);
}

bool StackedWidgetData::initializeAnimation()
{
    CT_SYSLOG(LOG_DEBUG, "");
    // check enability
    if (!(_target && _target.data()->isVisible())) {
        return false;
    }

    // check index
    if (_target.data()->currentIndex() == _index) {
        return false;
    }

    // do not animate if either index or currentIndex is not valid
    // but update _index none the less
    if (_target.data()->currentIndex() < 0 || _index < 0) {
        _index = _target.data()->currentIndex();
        return false;
    }

    // get old widget (matching _index) and initialize transition
    if (QWidget *widget = _target.data()->widget(_index)) {
        transition().data()->setOpacity(0);
        startClock();
        transition().data()->setGeometry(widget->geometry());
        transition().data()->setStartPixmap(transition().data()->grab(widget));

        _index = _target.data()->currentIndex();
        return !slow();
    } else {
        _index = _target.data()->currentIndex();
        return false;
    }
}

bool StackedWidgetData::animate()
{
    CT_SYSLOG(LOG_DEBUG, "");
    // check enability
    if (!enabled()) {
        return false;
    }

    // initialize animation
    if (!initializeAnimation()) {
        return false;
    }

    // show transition widget
    transition().data()->show();
    transition().data()->raise();
    transition().data()->animate();
    return true;
}

void StackedWidgetData::finishAnimation()
{
    CT_SYSLOG(LOG_DEBUG, "");
    // disable updates on currentWidget
    if (_target && _target.data()->currentWidget()) {
        _target.data()->currentWidget()->setUpdatesEnabled(false);
    }

    // hide transition
    transition().data()->hide();

    // reenable updates and repaint
    if (_target && _target.data()->currentWidget()) {
        _target.data()->currentWidget()->setUpdatesEnabled(true);
        _target.data()->currentWidget()->repaint();
    }

    // invalidate start widget
    transition().data()->resetStartPixmap();
}

void StackedWidgetData::targetDestroyed()
{
    CT_SYSLOG(LOG_DEBUG, "");
    setEnabled(false);
    _target.clear();
}

}
