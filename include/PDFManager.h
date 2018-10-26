#include <QImage>
#include <QPixmap>
#include <QLibrary>
#include <QGraphicsView>

#include <poppler-qt5.h>

class PDFManager: public QGraphicsView
{
    Q_OBJECT

public:
    PDFManager();
    ~PDFManager();

    bool Init(QString fileName);
    int getPageCount();
    Poppler::Page* getPage(int pageNumber);
    QSize getPageSize(int pageNumber);
    QImage getPageImage(int pageNumber);

    const QString fileName;

private:
    Poppler::Document* document;
    Poppler::Page* page;
    QList<Poppler::Page*> documentPages;
};
