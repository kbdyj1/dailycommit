#include <QImage>
#include <QDebug>
#include <QColorSpace>

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

            if (copy.isNull())
                copy = image;
        }
    }
    qDebug() << "cacheKey: " << copy.cacheKey();
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_image(const QStringList& filenames)
{
    _1::test(filenames);
}
