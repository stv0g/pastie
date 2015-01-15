/* Library:   CTK
 *
 * Copyright (c) Kitware Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 */

// Qt includes
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionSlider>
#include <QApplication>
#include <QStylePainter>
#include <QStyle>
#include <QToolTip>

#include "RangeSlider.h"

class RangeSliderPrivate
{
    Q_DECLARE_PUBLIC(RangeSlider);

protected:
    RangeSlider* const q_ptr;

public:
    /// Boolean indicates the selected handle
    ///   True for the minimum range handle, false for the maximum range handle
    enum Handle {
        NoHandle = 0x0,
        MinimumHandle = 0x1,
        MaximumHandle = 0x2
    };
    Q_DECLARE_FLAGS(Handles, Handle);

    RangeSliderPrivate(RangeSlider& object);
    void init();

    /// Return the handle at the given pos, or none if no handle is at the pos.
    /// If a handle is selected, handleRect is set to the handle rect.
    /// otherwise return NoHandle and handleRect is set to the combined rect of
    /// the min and max handles
    Handle handleAtPos(const QPoint& pos, QRect& handleRect) const;

    /// Copied verbatim from QSliderPrivate class (see QSlider.cpp)
    int pixelPosToRangeValue(int pos) const;
    int pixelPosFromRangeValue(int val) const;

    /// Draw the bottom and top sliders.
    void drawHandle(QStylePainter* painter, enum Handle handle) const;
    
    /// End points of the range on the Model
    int maximumValue;
    int minimumValue;

    /// End points of the range on the GUI. This is synced with the model.
    int maximumPosition;
    int minimumPosition;

    /// Controls selected ?
    QStyle::SubControl minimumSliderSelected;
    QStyle::SubControl maximumSliderSelected;

    /// See QSliderPrivate::clickOffset.
    /// Overrides this ivar
    int subclassClickOffset;
    
    /// See QSliderPrivate::position
    /// Overrides this ivar.
    int subclassPosition;

    /// Original width between the 2 bounds before any moves
    float subclassWidth;

    RangeSliderPrivate::Handles selectedHandles;

    QString handleToolTip;

private:
    Q_DISABLE_COPY(RangeSliderPrivate);
};

RangeSliderPrivate::RangeSliderPrivate(RangeSlider& object) :
    q_ptr(&object),
    maximumValue(100),
    minimumValue(0),
    maximumPosition(100),
    minimumPosition(0),
    minimumSliderSelected(QStyle::SC_None),
    maximumSliderSelected(QStyle::SC_None),
    subclassClickOffset(0),
    subclassPosition(0),
    subclassWidth(0),
    selectedHandles(0)
{ }

void RangeSliderPrivate::init()
{
    Q_Q(RangeSlider);
    minimumValue = q->minimum();
    maximumValue = q->maximum();
    minimumPosition = q->minimum();
    maximumPosition = q->maximum();
    q->connect(q, SIGNAL(rangeChanged(int,int)), q, SLOT(onRangeChanged(int,int)));
}

RangeSliderPrivate::Handle RangeSliderPrivate::handleAtPos(const QPoint& pos, QRect& handleRect) const
{
    Q_Q(const RangeSlider);

    QStyleOptionSlider option;
    q->initStyleOption(&option);

    // The functinos hitTestComplexControl only know about 1 handle. As we have
    // 2, we change the position of the handle and test if the pos correspond to
    // any of the 2 positions.

    // Test the MinimumHandle
    option.sliderPosition = minimumPosition;
    option.sliderValue    = minimumValue;

    QStyle::SubControl minimumControl = q->style()->hitTestComplexControl(
                QStyle::CC_Slider, &option, pos, q);
    QRect minimumHandleRect = q->style()->subControlRect(
                QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);

    // Test if the pos is under the Maximum handle
    option.sliderPosition = maximumPosition;
    option.sliderValue    = maximumValue;

    QStyle::SubControl maximumControl = q->style()->hitTestComplexControl(
                QStyle::CC_Slider, &option, pos, q);
    QRect maximumHandleRect = q->style()->subControlRect(
                QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, q);

    // The pos is above both handles, select the closest handle
    if (minimumControl == QStyle::SC_SliderHandle &&
            maximumControl == QStyle::SC_SliderHandle) {
        int minDist = 0;
        int maxDist = 0;
        if (q->orientation() == Qt::Horizontal) {
            minDist = pos.x() - minimumHandleRect.left();
            maxDist = maximumHandleRect.right() - pos.x();
        }
        else if (q->orientation() == Qt::Vertical) {
            minDist = minimumHandleRect.bottom() - pos.y();
            maxDist = pos.y() - maximumHandleRect.top();
        }
        Q_ASSERT(minDist >= 0 && maxDist >= 0);
        minimumControl = minDist < maxDist ? minimumControl : QStyle::SC_None;
    }

    if (minimumControl == QStyle::SC_SliderHandle) {
        handleRect = minimumHandleRect;
        return MinimumHandle;
    }
    else if (maximumControl == QStyle::SC_SliderHandle) {
        handleRect = maximumHandleRect;
        return MaximumHandle;
    }

    handleRect = minimumHandleRect.united(maximumHandleRect);
    return NoHandle;
}

int RangeSliderPrivate::pixelPosToRangeValue(int pos) const
{
    Q_Q(const RangeSlider);
    QStyleOptionSlider option;
    q->initStyleOption(&option);

    QRect gr = q->style()->subControlRect(QStyle::CC_Slider,
                                           &option,
                                           QStyle::SC_SliderGroove,
                                           q);
    QRect sr = q->style()->subControlRect(QStyle::CC_Slider,
                                           &option,
                                           QStyle::SC_SliderHandle,
                                           q);
    int sliderMin, sliderMax, sliderLength;
    if (option.orientation == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    }
    else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }

    return QStyle::sliderValueFromPosition(q->minimum(),
                                            q->maximum(),
                                            pos - sliderMin,
                                            sliderMax - sliderMin,
                                            option.upsideDown);
}

int RangeSliderPrivate::pixelPosFromRangeValue(int val) const
{
    Q_Q(const RangeSlider);
    QStyleOptionSlider option;
    q->initStyleOption(&option);

    QRect gr = q->style()->subControlRect(QStyle::CC_Slider,
                                           &option,
                                           QStyle::SC_SliderGroove,
                                           q);
    QRect sr = q->style()->subControlRect(QStyle::CC_Slider,
                                           &option,
                                           QStyle::SC_SliderHandle,
                                           q);
    int sliderMin, sliderMax, sliderLength;
    if (option.orientation == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    }
    else
    {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }

    return QStyle::sliderPositionFromValue(q->minimum(),
                                            q->maximum(),
                                            val,
                                            sliderMax - sliderMin,
                                            option.upsideDown) + sliderMin;
}

void RangeSliderPrivate::drawHandle(QStylePainter* painter, enum Handle handle) const
{
    Q_Q(const RangeSlider);
    QStyleOptionSlider option;
    q->initMinimumSliderStyleOption(&option);

    option.subControls = QStyle::SC_SliderHandle;
    option.sliderValue = (handle == MinimumHandle) ? minimumValue : maximumValue;
    option.sliderPosition = (handle == MinimumHandle) ? minimumPosition : maximumPosition;

    if ((handle == MinimumHandle && q->isMinimumSliderDown()) ||
        (handle == MaximumHandle && q->isMinimumSliderDown())) {
        option.activeSubControls = QStyle::SC_SliderHandle;
        option.state |= QStyle::State_Sunken;
    }

#ifdef Q_OS_MAC
    // On mac style, drawing just the handle actually draws also the groove.
    QRect clip = q->style()->subControlRect(QStyle::CC_Slider, &option,
                                            QStyle::SC_SliderHandle, q);
    clip.adjust(+2, +2, -2, -2);
    painter->setClipRect(clip);
#endif
    painter->drawComplexControl(QStyle::CC_Slider, option);
}

RangeSlider::RangeSlider(QWidget* _parent) :
    QSlider(_parent),
    d_ptr(new RangeSliderPrivate(*this))
{
    Q_D(RangeSlider);
    d->init();
}

RangeSlider::RangeSlider(Qt::Orientation o, QWidget* parentObject) :
    QSlider(o, parentObject),
    d_ptr(new RangeSliderPrivate(*this))
{
    Q_D(RangeSlider);
    d->init();
}

RangeSlider::RangeSlider(RangeSliderPrivate* impl, QWidget* _parent) :
    QSlider(_parent),
    d_ptr(impl)
{
    Q_D(RangeSlider);
    d->init();
}

RangeSlider::RangeSlider(RangeSliderPrivate* impl, Qt::Orientation o, QWidget* parentObject) :
    QSlider(o, parentObject),
    d_ptr(impl)
{
    Q_D(RangeSlider);
    d->init();
}

RangeSlider::~RangeSlider()
{ }

int RangeSlider::minimumValue() const
{
    Q_D(const RangeSlider);
    return d->minimumValue;
}

void RangeSlider::setMinimumValue(int min)
{
    Q_D(RangeSlider);
    setValues(min, qMax(d->maximumValue,min));
}

int RangeSlider::maximumValue() const
{
    Q_D(const RangeSlider);
    return d->maximumValue;
}

void RangeSlider::setMaximumValue(int max)
{
    Q_D(RangeSlider);
    setValues(qMin(d->minimumValue, max), max);
}

void RangeSlider::setValues(int l, int u)
{
    Q_D(RangeSlider);
    const int minValue = qBound(minimum(), qMin(l,u), maximum());
    const int maxValue =  qBound(minimum(), qMax(l,u), maximum());

    bool emitMinValChanged = (minValue != d->minimumValue);
    bool emitMaxValChanged = (maxValue != d->maximumValue);

    d->minimumValue = minValue;
    d->maximumValue = maxValue;

    bool emitMinPosChanged = (minValue != d->minimumPosition);
    bool emitMaxPosChanged = (maxValue != d->maximumPosition);

    d->minimumPosition = minValue;
    d->maximumPosition = maxValue;

    if (isSliderDown()) {
        if (emitMinPosChanged)
            emit minimumPositionChanged(d->minimumPosition);
        if (emitMaxPosChanged)
            emit maximumPositionChanged(d->maximumPosition);
		if (emitMaxPosChanged || emitMinPosChanged)
			emit positionsChanged(d->minimumPosition, d->maximumPosition);
    }

    if (emitMinValChanged)
        emit minimumValueChanged(d->minimumValue);
    if (emitMaxValChanged)
        emit maximumValueChanged(d->maximumValue);
	if (emitMaxValChanged || emitMinValChanged)
		emit valuesChanged(d->minimumValue, d->maximumValue);

    if (emitMinPosChanged || emitMaxPosChanged ||
            emitMinValChanged || emitMaxValChanged) {
        update();
    }
}

int RangeSlider::minimumPosition() const
{
    Q_D(const RangeSlider);
    return d->minimumPosition;
}

int RangeSlider::maximumPosition() const
{
    Q_D(const RangeSlider);
    return d->maximumPosition;
}

void RangeSlider::setMinimumPosition(int l)
{
    Q_D(const RangeSlider);
    setPositions(l, qMax(l, d->maximumPosition));
}

void RangeSlider::setMaximumPosition(int u)
{
    Q_D(const RangeSlider);
    setPositions(qMin(d->minimumPosition, u), u);
}

void RangeSlider::setPositions(int min, int max)
{
    Q_D(RangeSlider);
	const int minPosition = qBound(minimum(), qMin(min, max), maximum());
	const int maxPosition = qBound(minimum(), qMax(min, max), maximum());

    bool emitMinPosChanged = (minPosition != d->minimumPosition);
    bool emitMaxPosChanged = (maxPosition != d->maximumPosition);

    if (!emitMinPosChanged && !emitMaxPosChanged)
        return;

    d->minimumPosition = minPosition;
    d->maximumPosition = maxPosition;

    if (!hasTracking())
        update();

    if (isSliderDown()) {
        if (emitMinPosChanged)
            emit minimumPositionChanged(d->minimumPosition);
        if (emitMaxPosChanged)
            emit maximumPositionChanged(d->maximumPosition);
		if (emitMaxPosChanged || emitMinPosChanged)
			emit positionsChanged(d->minimumPosition, d->maximumPosition);
    }

    if (hasTracking()) {
        triggerAction(SliderMove);
        setValues(d->minimumPosition, d->maximumPosition);
    }
}

void RangeSlider::onRangeChanged(int _minimum, int _maximum)
{
    Q_UNUSED(_minimum);
    Q_UNUSED(_maximum);
    Q_D(RangeSlider);
    setValues(d->minimumValue, d->maximumValue);
}

void RangeSlider::paintEvent(QPaintEvent*)
{
    Q_D(RangeSlider);
    QStyleOptionSlider option;
    initStyleOption(&option);

    QStylePainter painter(this);
    option.subControls = QStyle::SC_SliderGroove;
    // Move to minimum to not highlight the SliderGroove.
    // On mac style, drawing just the slider groove also draws the handles,
    // therefore we give a negative (outside of view) position.
    option.sliderValue = minimum() - maximum();
    option.sliderPosition = minimum() - maximum();
    painter.drawComplexControl(QStyle::CC_Slider, option);

    option.sliderPosition = d->minimumPosition;
    const QRect lr = style()->subControlRect(QStyle::CC_Slider,
                                              &option,
                                              QStyle::SC_SliderHandle,
                                              this);
    option.sliderPosition = d->maximumPosition;

    const QRect ur = style()->subControlRect(QStyle::CC_Slider,
                                              &option,
                                              QStyle::SC_SliderHandle,
                                              this);

    QRect groove = style()->subControlRect(QStyle::CC_Slider,
                                        &option,
                                        QStyle::SC_SliderGroove,
                                        this);

    QRect rangeBox;
    if (option.orientation == Qt::Horizontal)
        rangeBox = QRect(
                    QPoint(qMin(lr.center().x(), ur.center().x()), groove.center().y() - 2),
                    QPoint(qMax(lr.center().x(), ur.center().x()), groove.center().y() + 1));
    else
        rangeBox = QRect(
                    QPoint(groove.center().x() - 2, qMin(lr.center().y(), ur.center().y())),
                    QPoint(groove.center().x() + 1, qMax(lr.center().y(), ur.center().y())));


    groove.adjust(0, 0, -1, 0);

    // Create default colors based on the transfer function.
    QColor highlight = palette().color(QPalette::Normal, QPalette::Highlight);
    QLinearGradient gradient;
    if (option.orientation == Qt::Horizontal) {
        gradient = QLinearGradient(groove.center().x(), groove.top(),
                                    groove.center().x(), groove.bottom());
    }
    else {
        gradient = QLinearGradient(groove.left(), groove.center().y(),
                                    groove.right(), groove.center().y());
    }

    gradient.setColorAt(0, highlight.darker(120));
    gradient.setColorAt(1, highlight.lighter(160));

    painter.setPen(QPen(highlight.darker(150), 0));
    painter.setBrush(gradient);
    painter.drawRect(rangeBox.intersected(groove));

    if (isMinimumSliderDown()) {
        d->drawHandle(&painter, RangeSliderPrivate::MaximumHandle);
        d->drawHandle(&painter, RangeSliderPrivate::MinimumHandle);
    }
    else {
        d->drawHandle(&painter, RangeSliderPrivate::MinimumHandle);
        d->drawHandle(&painter, RangeSliderPrivate::MaximumHandle);
    }
}

void RangeSlider::mousePressEvent(QMouseEvent* mouseEvent)
{
    Q_D(RangeSlider);
    if (minimum() == maximum() || (mouseEvent->buttons() ^ mouseEvent->button())) {
        mouseEvent->ignore();
        return;
    }

    int mepos = orientation() == Qt::Horizontal ?
                mouseEvent->pos().x() : mouseEvent->pos().y();

    QStyleOptionSlider option;
    initStyleOption(&option);

    QRect handleRect;
    RangeSliderPrivate::Handle handle_ = d->handleAtPos(mouseEvent->pos(), handleRect);

    if (handle_ != RangeSliderPrivate::NoHandle) {
        d->subclassPosition = (handle_ == RangeSliderPrivate::MinimumHandle)?
                    d->minimumPosition : d->maximumPosition;

        // save the position of the mouse inside the handle for later
        d->subclassClickOffset = mepos - (orientation() == Qt::Horizontal ?
                                              handleRect.left() : handleRect.top());

        setSliderDown(true);

        if (d->selectedHandles != handle_) {
            d->selectedHandles = handle_;
            update(handleRect);
        }
        // Accept the mouseEvent
        mouseEvent->accept();
        return;
    }

    // if we are here, no handles have been pressed
    // Check if we pressed on the groove between the 2 handles

    QStyle::SubControl control = style()->hitTestComplexControl(
                QStyle::CC_Slider, &option, mouseEvent->pos(), this);
    QRect sr = style()->subControlRect(
                QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);
    int minCenter = (orientation() == Qt::Horizontal ?
                         handleRect.left() : handleRect.top());
    int maxCenter = (orientation() == Qt::Horizontal ?
                         handleRect.right() : handleRect.bottom());
    if (control == QStyle::SC_SliderGroove && mepos > minCenter && mepos < maxCenter) {
        // warning lost of precision it might be fatal
        d->subclassPosition = (d->minimumPosition + d->maximumPosition) / 2.;
        d->subclassClickOffset = mepos - d->pixelPosFromRangeValue(d->subclassPosition);
        d->subclassWidth = (d->maximumPosition - d->minimumPosition) / 2.;
        qMax(d->subclassPosition - d->minimumPosition, d->maximumPosition - d->subclassPosition);
        setSliderDown(true);
        if (!isMinimumSliderDown() || !isMaximumSliderDown()) {
            d->selectedHandles =
                    QFlags<RangeSliderPrivate::Handle>(RangeSliderPrivate::MinimumHandle) |
                    QFlags<RangeSliderPrivate::Handle>(RangeSliderPrivate::MaximumHandle);
            update(handleRect.united(sr));
        }

        mouseEvent->accept();
        return;
    }
    mouseEvent->ignore();
}

void RangeSlider::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    Q_D(RangeSlider);
    if (!d->selectedHandles) {
        mouseEvent->ignore();
        return;
    }
    int mepos = orientation() == Qt::Horizontal ?
                mouseEvent->pos().x() : mouseEvent->pos().y();

    QStyleOptionSlider option;
    initStyleOption(&option);

    const int m = style()->pixelMetric(QStyle::PM_MaximumDragDistance, &option, this);

    int newPosition = d->pixelPosToRangeValue(mepos - d->subclassClickOffset);

    if (m >= 0) {
        const QRect r = rect().adjusted(-m, -m, m, m);
        if (!r.contains(mouseEvent->pos()))
            newPosition = d->subclassPosition;
    }

    if (isMinimumSliderDown() && !isMaximumSliderDown())  {
        double newMinPos = qMin(newPosition,d->maximumPosition);
        setPositions(newMinPos, d->maximumPosition);
    }
    else if (isMaximumSliderDown() && !isMinimumSliderDown()) {
        double newMaxPos = qMax(d->minimumPosition, newPosition);
        setPositions(d->minimumPosition, newMaxPos);
    }
    else if (isMinimumSliderDown() && isMaximumSliderDown()) {
        setPositions(newPosition - static_cast<int>(d->subclassWidth),
                     newPosition + static_cast<int>(d->subclassWidth + .5));
    }

    mouseEvent->accept();
}

void RangeSlider::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    Q_D(RangeSlider);
    QSlider::mouseReleaseEvent(mouseEvent);

    setSliderDown(false);
    d->selectedHandles = 0;

    setValues(d->minimumPosition, d->maximumPosition);
}

bool RangeSlider::isMinimumSliderDown() const
{
    Q_D(const RangeSlider);
    return d->selectedHandles & RangeSliderPrivate::MinimumHandle;
}

bool RangeSlider::isMaximumSliderDown() const
{
    Q_D(const RangeSlider);
    return d->selectedHandles & RangeSliderPrivate::MaximumHandle;
}

void RangeSlider::initMinimumSliderStyleOption(QStyleOptionSlider* option) const
{
    initStyleOption(option);
}

void RangeSlider::initMaximumSliderStyleOption(QStyleOptionSlider* option) const
{
    initStyleOption(option);
}
