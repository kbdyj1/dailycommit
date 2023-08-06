#include <QHttpMultiPart>

namespace { //=================================================================

namespace _1 {

void test()
{
    auto textPart = QHttpPart{};
    textPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant{"text/plain"});
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant{"form-data; name=\"text\""});
    textPart.setBody("Body");
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_multipart()
{
    _1::test();
}
