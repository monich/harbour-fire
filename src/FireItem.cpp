/*
 * Copyright (C) 2022-2023 Slava Monich <slava@monich.com>
 * Copyright (C) 2022 Jolla Ltd.
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

#include "FireItem.h"

#include "HarbourDebug.h"

#include <QTimer>
#include <QPainter>
#include <QtMath>

// ==========================================================================
// FireItem::Private
// ==========================================================================

class FireItem::Private :
    public QObject
{
    Q_OBJECT

public:
    static const qreal MinIntensity;
    static const qreal MaxIntensity;
    static const qreal MinThreshold;
    static const qreal MaxThreshold;
    static const qreal kMinWind;
    static const qreal kMaxWind;
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
    qreal iWind;
    uint iRand;
};

const qreal FireItem::Private::MinIntensity = 0.0;
const qreal FireItem::Private::MaxIntensity = 1.0;
const qreal FireItem::Private::MinThreshold = 0.5;
const qreal FireItem::Private::MaxThreshold = 0.8;
const qreal FireItem::Private::kMinWind = -1.0;
const qreal FireItem::Private::kMaxWind = 1.0;
const uint FireItem::Private::BlackPixel = 0;
const QRgb FireItem::Private::ColorTable[] = {
    #define C1(i) qRgb(0, 0, i << 1)
    #define C2(i) qRgb(i << 3, 0, 64 - (i << 1))
    #define C3(i) qRgb(255, i << 3, 0)
    #define C4(i) qRgb(255, 255, i << 2)
    #define C5(i) qRgb(255, 255, 64 + (i << 2))
    #define C6(i) qRgb(255, 255, 128 + (i << 2))
    #define C7(i) qRgb(255, 255, 192 + i)
    #define C8(i) qRgb(255, 255, 224 + i)
    #define x32(x) x(0),x(1),x(2),x(3),x(4),x(5),x(6),x(7),x(8),x(9), \
        x(10),x(11),x(12),x(13),x(14),x(15),x(16),x(17),x(18),x(19), \
        x(20),x(21),x(22),x(23),x(24),x(25),x(26),x(27),x(28),x(29), \
        x(30),x(31)
    x32(C1), x32(C2), x32(C3), x32(C4), x32(C5), x32(C6), x32(C7), x32(C8)
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
    iWind((kMinWind + kMaxWind) / 2),
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
    HDEBUG(iThreshold);
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
    const qreal b = qAbs(iWind);
    const qreal d = 4 + b;

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
        if (iWind >= 0) {
            for (int x = left + 1; x < right; x++) {
                row[x] = colorIndex((row[x - 1] * b + row[x] +
                    lower[x - 1] + lower[x] + lower[x + 1])/d);
            }
        } else {
            for (int x = right - 1; x > left; x--) {
                row[x] = colorIndex((row[x + 1] * b + row[x] +
                    lower[x - 1] + lower[x] + lower[x + 1])/d);
            }
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
        HDEBUG(value);
        iPrivate->iThreshold = (iPrivate->minThreshold() + Private::MaxThreshold) / 2;
        iPrivate->randomizeThreshold();
        Q_EMIT intensityChanged();
    }
}

qreal
FireItem::wind() const
{
    return iPrivate->iWind;
}

void
FireItem::setWind(
    qreal aValue)
{
    const qreal value = qMin(qMax(aValue, Private::kMinWind), Private::kMaxWind);

    if (iPrivate->iWind != value) {
        iPrivate->iWind = value;
        HDEBUG(value);
        Q_EMIT windChanged();
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
