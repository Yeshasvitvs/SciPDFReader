#include "PDFManager.h"

PDFManager::PDFManager(): document(nullptr), page(nullptr)
{

}

PDFManager::~PDFManager()
{

}

bool PDFManager::Init(QString fileName)
{
    document = Poppler::Document::load(fileName);

    document->setRenderHint(Poppler::Document::Antialiasing);
    document->setRenderHint(Poppler::Document::TextAntialiasing);

    return true;
}

int PDFManager::getPageCount()
{
    int numberOfPages = 0;

    if(document != nullptr) {
        numberOfPages = document->numPages();
    }

    return numberOfPages;
}

Poppler::Page* PDFManager::getPage(int pageNumer)
{
    Poppler::Page* page = nullptr;

    if(document != nullptr) {
        page = document->page(pageNumer);
        //Q_ASSERT(page != NULL);
    }

    return page;
}

QSize PDFManager::getPageSize(int pageNumber)
{
    Poppler::Page* page = nullptr;
    page = getPage(pageNumber);

    QSize size = QSize(0, 0);

    if (page != nullptr) {
        size = page->pageSize();
    }

    return size;
}

QImage PDFManager::getPageImage(int pageNumber)
{
    Poppler::Page* page = nullptr;

    page = getPage(pageNumber);

    QSize size = getPageSize(pageNumber);

    QImage pageImage;

    if (size.isValid() || !size.isEmpty()) {
        int width = size.width();
        int height = size.height();

        pageImage = page->renderToImage(72, 72, 0, pageNumber, width, height);
    }
    else {
        // Sending dummy image
        QImage image = QPixmap(100, 100).toImage();

        //TODO: Display error dialog
    }

    return pageImage;
}
