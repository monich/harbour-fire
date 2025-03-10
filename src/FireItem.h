/*
 * Copyright (C) 2022-2025 Slava Monich <slava@monich.com>
 * Copyright (C) 2022 Jolla Ltd.
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer
 *     in the documentation and/or other materials provided with the
 *     distribution.
 *
 *  3. Neither the names of the copyright holders nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * any official policies, either expressed or implied.
 */

#ifndef FIRE_ITEM_H
#define FIRE_ITEM_H

#include <QtGlobal>

#if QT_VERSION >= 0x050000
#  include <QQuickPaintedItem>
#  define FireItemBase QQuickPaintedItem
#  define FireItemParent QQuickItem
#else
#  include <QDeclarativeItem>
#  define FireItemBase QDeclarativeItem
#  define FireItemParent QDeclarativeItem
#endif

// The item becomes idle if it's not being repainted for 500ms.
// There's no need to pull updates from the sensors if the item isn't
// being repainted.
class FireItem:
    public FireItemBase
{
    Q_OBJECT
    Q_PROPERTY(bool idle READ idle NOTIFY idleChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(qreal intensity READ intensity WRITE setIntensity NOTIFY intensityChanged)
    Q_PROPERTY(qreal wind READ wind WRITE setWind NOTIFY windChanged)

public:
    explicit FireItem(FireItemParent* aParent = NULL);

    bool idle() const;
    bool active() const;
    void setActive(bool);

    qreal intensity() const;
    void setIntensity(qreal);

    qreal wind() const;
    void setWind(qreal);

protected:
    void paint(QPainter*);
#if QT_VERSION < 0x050000
    void paint(QPainter* aPainter, const QStyleOptionGraphicsItem*, QWidget*)
        { paint(aPainter); }
#endif

Q_SIGNALS:
    void idleChanged();
    void activeChanged();
    void intensityChanged();
    void windChanged();

private:
    class Private;
    Private* iPrivate;
};

#endif // FIRE_ITEM_H
