/* Library:   CTK
 *
 * Copyright (c) Kitware Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 */

#ifndef _RangeSlider_
#define _RangeSlider_

// Qt includes
#include <QSlider>

class QStylePainter;
class RangeSliderPrivate;

class RangeSlider : public QSlider
{
    Q_OBJECT
    Q_PROPERTY(int minimumValue READ minimumValue WRITE setMinimumValue)
    Q_PROPERTY(int maximumValue READ maximumValue WRITE setMaximumValue)
    Q_PROPERTY(int minimumPosition READ minimumPosition WRITE setMinimumPosition)
    Q_PROPERTY(int maximumPosition READ maximumPosition WRITE setMaximumPosition)

public:
    explicit RangeSlider( Qt::Orientation o, QWidget* par= 0 );
    explicit RangeSlider( QWidget* par = 0 );
    virtual ~RangeSlider();

    int minimumValue() const;
    int maximumValue() const;

    int minimumPosition() const;
    void setMinimumPosition(int min);

    int maximumPosition() const;
    void setMaximumPosition(int max);

    bool isMinimumSliderDown() const;
    bool isMaximumSliderDown() const;

    void setPositions(int min, int max);

Q_SIGNALS:
    void minimumValueChanged(int min);
    void maximumValueChanged(int max);

    void minimumPositionChanged(int min);
    void maximumPositionChanged(int max);

	void valuesChanged(int min, int max);
	void positionsChanged(int min, int max);

public Q_SLOTS:
    void setMinimumValue(int min);
    void setMaximumValue(int max);

    void setValues(int min, int max);

protected Q_SLOTS:
    void onRangeChanged(int minimum, int maximum);

protected:
    RangeSlider(RangeSliderPrivate* impl, Qt::Orientation o, QWidget* par= 0);
    RangeSlider(RangeSliderPrivate* impl, QWidget* par = 0);

    virtual void mousePressEvent(QMouseEvent* ev);
    virtual void mouseMoveEvent(QMouseEvent* ev);
    virtual void mouseReleaseEvent(QMouseEvent* ev);

    virtual void paintEvent(QPaintEvent* ev);
    virtual void initMinimumSliderStyleOption(QStyleOptionSlider* option) const;
    virtual void initMaximumSliderStyleOption(QStyleOptionSlider* option) const;

protected:
    QScopedPointer<RangeSliderPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(RangeSlider);
    Q_DISABLE_COPY(RangeSlider);
};

#endif
