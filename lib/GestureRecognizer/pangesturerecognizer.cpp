/* Copyright (C) 2015 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
 *
 * This file is part of eta-gestemas.
 *
 * eta-gestemas is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eta-gestemas is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with eta-gestemas.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pangesturerecognizer.h"
#include "touch.h"
#include "utilities.h"
#include <QtCore/QtDebug>

PanGestureRecognizer::PanGestureRecognizer()
    :GestureRecognizer(),
     m_maxNumTouchesRequired(2),
     m_minNumTouchesRequired(2),
     m_velocityX(0.0f),
     m_velocityY(0.0f),
     m_translationX(0.0f),
     m_translationY(0.0f)
{
}

bool PanGestureRecognizer::isEqual(const GestureRecognizer& other) const
{
    if (!GestureRecognizer::isEqual(other)) return false;

    const PanGestureRecognizer *p =
        static_cast<const PanGestureRecognizer*>(&other);
    if (m_maxNumTouchesRequired != p->m_maxNumTouchesRequired) return false;
    if (m_minNumTouchesRequired != p->m_minNumTouchesRequired) return false;

    return true;
}

void PanGestureRecognizer::onTouchBegan(const Touch *touch)
{
    if (numTouches() > maxNumTouchesRequired()) {
        if (state() == State::Possible) {
            setState(State::Failed);
        } else {
            ignoreTouch(touch);
        }
        return;
    }
    if (numTouches() >= minNumTouchesRequired()) {
        updateCentralPoint();
        m_timeStamp = touch->timeStamp();
    }
}

void PanGestureRecognizer::onTouchMoved(const Touch *touch)
{
    if (numTouches() < minNumTouchesRequired()) {
        return;
    }

    const Touch *t = touch;
    float displacement= 0.0f;
    uint64_t deltaTime = 0;
    if (state() == State::Possible) {
        m_prevCentralX = centralX();
        m_prevCentralY = centralY();
        updateCentralPoint();
        deltaTime = t->timeStamp() - m_timeStamp;
        deltaTime = (deltaTime == 0) ? 1 : deltaTime;
        m_timeStamp = t->timeStamp();
        if ((SQUARED(t->deltaX()) + SQUARED(t->deltaY())) >
            SQUARED(recognitionThreshold())) {
            displacement = centralX() - m_prevCentralX;
            m_velocityX = displacement / deltaTime;
            m_translationX = displacement;

            displacement = centralY() - m_prevCentralY;
            m_velocityY = displacement / deltaTime;
            m_translationY = displacement;

            setState(State::Began);
        }
    } else if (state() == State::Began || state() == State::Changed) {
        m_prevCentralX = centralX();
        m_prevCentralY = centralY();
        updateCentralPoint();
        deltaTime = t->timeStamp() - m_timeStamp;
        deltaTime = (deltaTime == 0) ? 1 : deltaTime;
        m_timeStamp = t->timeStamp();
        displacement = centralX() - m_prevCentralX;
        m_velocityX = displacement / deltaTime;
        m_translationX += displacement;

        displacement = centralY() - m_prevCentralY;
        m_velocityY = displacement / deltaTime;
        m_translationY += displacement;
        setState(State::Changed);
    }
}

void PanGestureRecognizer::onTouchEnded(const Touch *touch)
{
    (void)touch;
    if (numTouches() < minNumTouchesRequired()) {
        if (state() == State::Possible) {
            setState(State::Failed);
        } else if (state() == State::Began || state() == State::Changed) {
            setState(State::Ended);
        }
    } else {
        updateCentralPoint();
    }
}

void PanGestureRecognizer::reset()
{
    GestureRecognizer::reset();
}

float PanGestureRecognizer::velocity() const
{
    return sqrtf(SQUARED(m_velocityX) +
                 SQUARED(m_velocityY));
}
