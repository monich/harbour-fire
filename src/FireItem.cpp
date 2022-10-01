/*
 * Copyright (C) 2022 Jolla Ltd.
 * Copyright (C) 2022 Slava Monich <slava@monich.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer
 *      in the documentation and/or other materials provided with the
 *      distribution.
 *   3. Neither the names of the copyright holders nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
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

#include "FireDebug.h"
#include "FireItem.h"

#include <QTimer>
#include <QPainter>
#include <QtMath>

// ==========================================================================
// FireItem::Private
// ==========================================================================

class FireItem::Private : public QObject
{
    Q_OBJECT

public:
    static const qreal MinIntensity;
    static const qreal MaxIntensity;
    static const qreal MinThreshold;
    static const qreal MaxThreshold;
    static const uint BlackPixel;
    static const QRgb ColorTable[];

public:
    Private(FireItem*, int, int);

    FireItem* fireItem();
    qreal random();
    uchar colorIndex(qreal);
    qreal minThreshold();
    void randomizeThreshold();
    void updateImage();

public slots:
    void onRepaintTimer();

public:
    const QSize iSourceSize;
    QImage iImage;
    QVector<QRgb> iColorTable;
    QTimer* iRepaintTimer;
    qreal iIntensity;
    qreal iThreshold;
    uint iRand;
};

const qreal FireItem::Private::MinIntensity = 0.0;
const qreal FireItem::Private::MaxIntensity = 1.0;
const qreal FireItem::Private::MinThreshold = 0.5;
const qreal FireItem::Private::MaxThreshold = 0.8;
const uint FireItem::Private::BlackPixel = 0;
const QRgb FireItem::Private::ColorTable[] = {
    qRgb(0,0,0), qRgb(0,0,2), qRgb(0,0,4), qRgb(0,0,6),
    qRgb(0,0,8), qRgb(0,0,10), qRgb(0,0,12), qRgb(0,0,14),
    qRgb(0,0,16), qRgb(0,0,18), qRgb(0,0,20), qRgb(0,0,22),
    qRgb(0,0,24), qRgb(0,0,26), qRgb(0,0,28), qRgb(0,0,30),
    qRgb(0,0,32), qRgb(0,0,34), qRgb(0,0,36), qRgb(0,0,38),
    qRgb(0,0,40), qRgb(0,0,42), qRgb(0,0,44), qRgb(0,0,46),
    qRgb(0,0,48), qRgb(0,0,50), qRgb(0,0,52), qRgb(0,0,54),
    qRgb(0,0,56), qRgb(0,0,58), qRgb(0,0,60), qRgb(0,0,62),
    qRgb(0,0,64), qRgb(8,0,62), qRgb(16,0,60), qRgb(24,0,58),
    qRgb(32,0,56), qRgb(40,0,54), qRgb(48,0,52), qRgb(56,0,50),
    qRgb(64,0,48), qRgb(72,0,46), qRgb(80,0,44), qRgb(88,0,42),
    qRgb(96,0,40), qRgb(104,0,38), qRgb(112,0,36), qRgb(120,0,34),
    qRgb(128,0,32), qRgb(136,0,30), qRgb(144,0,28), qRgb(152,0,26),
    qRgb(160,0,24), qRgb(168,0,22), qRgb(176,0,20), qRgb(184,0,18),
    qRgb(192,0,16), qRgb(200,0,14), qRgb(208,0,12), qRgb(216,0,10),
    qRgb(224,0,8), qRgb(232,0,6), qRgb(240,0,4), qRgb(248,0,2),
    qRgb(255,0,0), qRgb(255,8,0), qRgb(255,16,0), qRgb(255,24,0),
    qRgb(255,32,0), qRgb(255,40,0), qRgb(255,48,0), qRgb(255,56,0),
    qRgb(255,64,0), qRgb(255,72,0), qRgb(255,80,0), qRgb(255,88,0),
    qRgb(255,96,0), qRgb(255,104,0), qRgb(255,112,0), qRgb(255,120,0),
    qRgb(255,128,0), qRgb(255,136,0), qRgb(255,144,0), qRgb(255,152,0),
    qRgb(255,160,0), qRgb(255,168,0), qRgb(255,176,0), qRgb(255,184,0),
    qRgb(255,192,0), qRgb(255,200,0), qRgb(255,208,0), qRgb(255,216,0),
    qRgb(255,224,0), qRgb(255,232,0), qRgb(255,240,0), qRgb(255,248,0),
    qRgb(255,255,0), qRgb(255,255,4), qRgb(255,255,8), qRgb(255,255,12),
    qRgb(255,255,16), qRgb(255,255,20), qRgb(255,255,24), qRgb(255,255,28),
    qRgb(255,255,32), qRgb(255,255,36), qRgb(255,255,40), qRgb(255,255,44),
    qRgb(255,255,48), qRgb(255,255,52), qRgb(255,255,56), qRgb(255,255,60),
    qRgb(255,255,64), qRgb(255,255,68), qRgb(255,255,72), qRgb(255,255,76),
    qRgb(255,255,80), qRgb(255,255,84), qRgb(255,255,88), qRgb(255,255,92),
    qRgb(255,255,96), qRgb(255,255,100), qRgb(255,255,104), qRgb(255,255,108),
    qRgb(255,255,112), qRgb(255,255,116), qRgb(255,255,120), qRgb(255,255,124),
    qRgb(255,255,64), qRgb(255,255,68), qRgb(255,255,72), qRgb(255,255,76),
    qRgb(255,255,80), qRgb(255,255,84), qRgb(255,255,88), qRgb(255,255,92),
    qRgb(255,255,96), qRgb(255,255,100), qRgb(255,255,104), qRgb(255,255,108),
    qRgb(255,255,112), qRgb(255,255,116), qRgb(255,255,120), qRgb(255,255,124),
    qRgb(255,255,128), qRgb(255,255,132), qRgb(255,255,136), qRgb(255,255,140),
    qRgb(255,255,144), qRgb(255,255,148), qRgb(255,255,152), qRgb(255,255,156),
    qRgb(255,255,160), qRgb(255,255,164), qRgb(255,255,168), qRgb(255,255,172),
    qRgb(255,255,176), qRgb(255,255,180), qRgb(255,255,184), qRgb(255,255,188),
    qRgb(255,255,128), qRgb(255,255,132), qRgb(255,255,136), qRgb(255,255,140),
    qRgb(255,255,144), qRgb(255,255,148), qRgb(255,255,152), qRgb(255,255,156),
    qRgb(255,255,160), qRgb(255,255,164), qRgb(255,255,168), qRgb(255,255,172),
    qRgb(255,255,176), qRgb(255,255,180), qRgb(255,255,184), qRgb(255,255,188),
    qRgb(255,255,192), qRgb(255,255,196), qRgb(255,255,200), qRgb(255,255,204),
    qRgb(255,255,208), qRgb(255,255,212), qRgb(255,255,216), qRgb(255,255,220),
    qRgb(255,255,224), qRgb(255,255,228), qRgb(255,255,232), qRgb(255,255,236),
    qRgb(255,255,240), qRgb(255,255,244), qRgb(255,255,248), qRgb(255,255,252),
    qRgb(255,255,192), qRgb(255,255,193), qRgb(255,255,194), qRgb(255,255,195),
    qRgb(255,255,196), qRgb(255,255,197), qRgb(255,255,198), qRgb(255,255,199),
    qRgb(255,255,200), qRgb(255,255,201), qRgb(255,255,202), qRgb(255,255,203),
    qRgb(255,255,204), qRgb(255,255,205), qRgb(255,255,206), qRgb(255,255,207),
    qRgb(255,255,208), qRgb(255,255,209), qRgb(255,255,210), qRgb(255,255,211),
    qRgb(255,255,212), qRgb(255,255,213), qRgb(255,255,214), qRgb(255,255,215),
    qRgb(255,255,216), qRgb(255,255,217), qRgb(255,255,218), qRgb(255,255,219),
    qRgb(255,255,220), qRgb(255,255,221), qRgb(255,255,222), qRgb(255,255,223),
    qRgb(255,255,224), qRgb(255,255,225), qRgb(255,255,226), qRgb(255,255,227),
    qRgb(255,255,228), qRgb(255,255,229), qRgb(255,255,230), qRgb(255,255,231),
    qRgb(255,255,232), qRgb(255,255,233), qRgb(255,255,234), qRgb(255,255,235),
    qRgb(255,255,236), qRgb(255,255,237), qRgb(255,255,238), qRgb(255,255,239),
    qRgb(255,255,240), qRgb(255,255,241), qRgb(255,255,242), qRgb(255,255,243),
    qRgb(255,255,244), qRgb(255,255,245), qRgb(255,255,246), qRgb(255,255,247),
    qRgb(255,255,248), qRgb(255,255,249), qRgb(255,255,250), qRgb(255,255,251),
    qRgb(255,255,252), qRgb(255,255,253), qRgb(255,255,254), qRgb(255,255,255)
};

FireItem::Private::Private(
    FireItem* aParent,
    int aWidth,
    int aHeight) :
    QObject(aParent),
    // The full height includes two invisible rows at the bottom
    iSourceSize(aWidth, aHeight),
    iImage(aWidth, aHeight + 2, QImage::Format_Indexed8),
    iRepaintTimer(new QTimer(this)),
    iIntensity((MinIntensity + MaxIntensity) / 2),
    iThreshold(MinThreshold),
    iRand(time(NULL))
{
    iColorTable.resize(sizeof(ColorTable)/sizeof(ColorTable[0]));
    memcpy(iColorTable.data(), ColorTable, sizeof(ColorTable));

    iImage.setColorTable(iColorTable);
    iImage.fill(BlackPixel);

    connect(iRepaintTimer, SIGNAL(timeout()), SLOT(onRepaintTimer()));
    iRepaintTimer->setInterval(40);
    iRepaintTimer->start();
}

FireItem*
FireItem::Private::fireItem()
{
    return qobject_cast<FireItem*>(parent());
}

void
FireItem::Private::onRepaintTimer()
{
    if (iImage.width() > 0) {
        updateImage();
    }
    fireItem()->update();
}

qreal
FireItem::Private::random()
{
    return qMin((qreal(rand_r(&iRand))/RAND_MAX), qreal(1.0));
}

qreal
FireItem::Private::minThreshold()
{
    return MinThreshold + (MaxThreshold - MinThreshold) *
        (MaxIntensity - iIntensity) / (MaxIntensity - MinIntensity);
}

void
FireItem::Private::randomizeThreshold()
{
    const qreal adjust = (MaxThreshold - MinThreshold) / 2;

    iThreshold = qMin(qMax(iThreshold + random() * adjust - adjust/2,
        minThreshold()), MaxThreshold);
    DBG(iThreshold);
}

uchar
FireItem::Private::colorIndex(
    qreal aValue)
{
    const uchar c = qFloor(aValue);

    return (c > 1) ? (c - 1) : c;
}

void
FireItem::Private::updateImage()
{
    const int w = iImage.width();

    // Randomize invisible bottom row
    int y = iImage.height() - 1;
    uchar* bottom = iImage.scanLine(y);

    for (int x = 0; x < w; x++) {
        bottom[x] = ((random() > iThreshold) ? 255 : 0);
    }

    // The rest of the image
    const int left = 0;
    const int right = w - 1;

    for (y--; y >= 0; y--) {
        const uchar* lower = iImage.constScanLine(y + 1);
        uchar* row = iImage.scanLine(y);

        // Left pixel
        row[left] = colorIndex(((uint)row[left] +
            lower[left] + lower[left + 1])/3.0);

        // Right pixel
        if (right > left) {
            row[right] = colorIndex(((uint)row[right] +
                lower[right - 1] + lower[right])/3.0);
        }

        // The rest of the line
        for (int x = left + 1; x < right; x++) {
            row[x] = colorIndex(((uint)row[x] +
                lower[x - 1] + lower[x] + lower[x + 1])/4.0);
        }
    }

    // Sometimes adjust the threshold
    if (random() > 0.95) {
        randomizeThreshold();
    }
}

// ==========================================================================
// FireItem
// ==========================================================================

FireItem::FireItem(
    QQuickItem* aParent) :
    QQuickPaintedItem(aParent),
    iPrivate(new Private(this, 320, 100))
{
    setFillColor(QColor(Qt::black));
}

bool
FireItem::active() const
{
    return iPrivate->iRepaintTimer->isActive();
}

void
FireItem::setActive(
    bool aActive)
{
    QTimer* repaintTimer = iPrivate->iRepaintTimer;

    if (aActive) {
        if (!repaintTimer->isActive()) {
            repaintTimer->start();
            Q_EMIT activeChanged();
        }
    } else if (repaintTimer->isActive()) {
        repaintTimer->stop();
        Q_EMIT activeChanged();
    }
}

qreal
FireItem::intensity() const
{
    return iPrivate->iIntensity;
}

void
FireItem::setIntensity(
    qreal aValue)
{
    const qreal value = qMin(qMax(aValue,
        Private::MinIntensity), Private::MaxIntensity);

    if (iPrivate->iIntensity != value) {
        iPrivate->iIntensity = value;
        DBG(value);
        iPrivate->iThreshold = (iPrivate->minThreshold() + Private::MaxThreshold) / 2;
        iPrivate->randomizeThreshold();
        Q_EMIT intensityChanged();
    }
}

void
FireItem::paint(
    QPainter* aPainter)
{
    const int w = width();
    const int h = height();
    const int minTargetHeight = h / 2;
    QRect sourceRect(QPoint(), iPrivate->iSourceSize);

    // Try to fill a reasonably large portion of the screen and
    // at the same time not to cut off too much
    if (sourceRect.width() * minTargetHeight > w * sourceRect.height()) {
        const int minSourceWidth = sourceRect.width() / 4;
        int partialWidth = sourceRect.height() * w / h;

        if (partialWidth >= minSourceWidth) {
            sourceRect.setWidth(partialWidth);
        } else {
            int sourceHeight = sourceRect.height();
            int destHeight = sourceHeight * w /minSourceWidth;

            sourceRect.setWidth(minSourceWidth);
            if (destHeight > h) {
                sourceRect.setY(sourceHeight * (destHeight - h) / destHeight);
            }
        }
    }

    const int targetHeight = w * sourceRect.height() / sourceRect.width();

    aPainter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    aPainter->drawImage(QRect(0, h - targetHeight, w, targetHeight),
        iPrivate->iImage, sourceRect);
}

#include "FireItem.moc"
