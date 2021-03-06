/* Copyright (C) 2015-2016 Gökhan Karabulut <gokhan.karabulut@tubitak.gov.tr>
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

#ifndef GESTURERECOGNIZER_H
#define GESTURERECOGNIZER_H

#include "state.h"
#include <QtCore/QObject>
#include <QtCore/QList>

class GestureRecognizerManager;
class Touch;
class GestureListener;

class GestureRecognizer : public QObject
{
    Q_OBJECT
public:
    explicit GestureRecognizer(QObject *parent = nullptr);
    virtual ~GestureRecognizer();
    virtual void reset();
    virtual bool isEqual(const GestureRecognizer& other) const;
    void touchBeganHandler(const Touch *touch);
    void touchMovedHandler(const Touch *touch);
    void touchEndedHandler(const Touch *touch);

    void callListener();

    void setManager(GestureRecognizerManager* manager);

    const State& state() const
    {return m_state;}

    void setRecognitionThreshold(float threshold)
    {m_recognitionThreshold = threshold;}
    float recognitionThreshold() const
    {return m_recognitionThreshold;}

    void setAllowSimultaneousRecognition(bool b)
    {m_allowSimultaneousRecognition = b;}
    bool allowsSimultaneousRecognition() const
    {return m_allowSimultaneousRecognition;}

    void setGestureRecognizersToAbort(const QList<GestureRecognizer*>& list)
    {m_gestureRecognizersToAbort = list;}
    const QList<GestureRecognizer*>& gestureRecognizersToAbort() const
    {return m_gestureRecognizersToAbort;}

    int numTouches() const
    {return m_touches.size();}

    void setTopMargin(float margin)
    {m_topMargin = margin;}

    void setBottomMargin(float margin)
    {m_bottomMargin = margin;}

    void setLeftMargin(float margin)
    {m_leftMargin = margin;}

    void setRightMargin(float margin)
    {m_rightMargin = margin;}

    void setCentralX(float x)
    {m_centralX = x;}
    float centralX() const
    {return m_centralX;}

    void setCentralY(float y)
    {m_centralY = y;}
    float centralY() const
    {return m_centralY;}

    void setId(int id)
    {m_id = id;}
    int id() const
    {return m_id;}

    uint32_t targetId() const
    {return m_targetId;}

    void setGestureListener(GestureListener *listener);
    const GestureListener* listener() const;

    GestureRecognizer(const GestureRecognizer&) = delete;
    GestureRecognizer& operator=(const GestureRecognizer&) = delete;

    static uint64_t samplingPeriod;
    static uint32_t pointerEmulationRate;
    static float pointerEmulationDistance;
protected:
    virtual void onTouchBegan(const Touch *touch) = 0;
    virtual void onTouchMoved(const Touch *touch) = 0;
    virtual void onTouchEnded(const Touch *touch) = 0;

    void handleTouchOwnership() const;

    void setState(const State& newState);

    void updateCentralPoint();

    void ignoreTouch(const Touch* touch);

    GestureRecognizerManager *m_manager;
    float m_topMargin;
    float m_bottomMargin;
    float m_leftMargin;
    float m_rightMargin;
    float m_centralX;
    float m_centralY;
    float m_recognitionThreshold;
    bool m_allowSimultaneousRecognition;
    QList<const Touch*> m_touches;
    QList<GestureRecognizer*> m_gestureRecognizersToAbort;
private:
    const Touch* findTouch(uint32_t touchId);

    GestureListener *m_listener;
    State m_state;
    QList<State> m_states;
    uint32_t m_targetId;
    int m_id;
};

#endif /* GESTURERECOGNIZER_H */
