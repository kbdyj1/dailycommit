#include <QImage>
#include <QDebug>
#include <QColorSpace>
#include <QFileInfo>
#include <QRgb>

namespace { //=================================================================

namespace _1 {

void test(const QStringList& filenames)
{
    QImage copy;

    for (const auto& filename : filenames) {
        QImage image{filename};
        if (image.isNull()) {
            qDebug() << "QImage{" << filename << "} load failed.";
        } else {
            qDebug() << "cacheKey: " << image.cacheKey() << ", size(" << image.width() << "x" << image.height() << ")";
            qDebug() << "colorSpace: " << image.colorSpace();
            qDebug() << "colorCount: " << image.colorCount();
            qDebug() << "devicePixelRatio: " << image.devicePixelRatio();
            qDebug() << "dotsPerMeterX: " << image.dotsPerMeterX();
            qDebug() << "dotsPerMeterY: " << image.dotsPerMeterY();

            if (copy.isNull())
                copy = image;
        }
    }
    qDebug() << "cacheKey: " << copy.cacheKey();
}

} //_1 --------------------------------------------------------------

namespace _2 {

void createHeuristicMask(QImage& image, const QString& filename)
{
    auto maskImage = image.createHeuristicMask();
    QFileInfo fi{filename};
    auto fn = fi.fileName();
    auto saveName = QString{"mask_%1"}.arg(fn);
    maskImage.save(saveName);
}

void createMaskFromColor(QImage& image, const QString& filename)
{
    auto maskColor = qRgb(0, 0, 0);
    auto maskImage = image.createMaskFromColor(maskColor);
    QFileInfo fi{filename};
    auto fn = fi.fileName();
    auto saveName = QString{"mask_%1"}.arg(fn);
    maskImage.save(saveName);
}

void invertImage(QImage& image, const QString& filename)
{
    image.invertPixels();
    QFileInfo fi{filename};
    auto fn = fi.fileName();
    auto saveName = QString{"inv_%1"}.arg(fn);
    image.save(saveName);
}

void test(const QStringList& filenames)
{
    for (const auto& filename : filenames) {
        QImage image{filename};
        if (false == image.isNull()) {
            //createHeuristicMask(image, filename);
            //createMaskFromColor(image, filename);
            invertImage(image, filename);
        }
    }
}

} //_2 --------------------------------------------------------------

} //===========================================================================

void test_image(const QStringList& filenames)
{
#if (0) //done
    _2::test(filenames);
#endif

    _1::test(filenames);
}
