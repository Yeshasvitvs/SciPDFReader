#include "mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent),
      textEdit(new QPlainTextEdit)
{
    // == WINDOW SETTINGS ==
    setWindowTitle("Basic Application");

    setMinimumSize(800, 450);

    // == Central Widget ==
    setCentralWidget(textEdit);

    createMenus();
    createToolBars();
    createActions();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);

    setCurrentFile(QString());
}

// == PRIVATE SLOTS ==
void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile(QString());
    }
    statusBar()->showMessage("File -> New");
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
    statusBar()->showMessage("File -> Open");
}

bool MainWindow::save()
{
    if (currentFile.isEmpty()) {
        return saveAs();
    }
    else {
        return saveFile(currentFile);
    }
    statusBar()->showMessage("File -> Save");
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }

    return saveFile(dialog.selectedFiles().first());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("Blah blah blah..."));
}

void MainWindow::documentWasModified()
{
    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::createMenus()
{
    fileMenu   = menuBar()->addMenu(tr("&File"));
    editMenu   = menuBar()->addMenu(tr("&File"));
    viewMenu   = menuBar()->addMenu(tr("&View"));
    windowMenu = menuBar()->addMenu(tr("&Windows"));
    helpMenu   = menuBar()->addMenu(tr("&Help"));
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    editToolBar = addToolBar(tr("Edit"));
    viewToolBar = addToolBar(tr("View"));
    navToolBar  = addToolBar(tr("Navigation"));
}

void MainWindow::createActions()
{

    // File menu actions
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    const QIcon closeIcon = QIcon::fromTheme("document-close");
    QAction *closeAct = fileMenu->addAction(closeIcon, tr("Close"), this, &MainWindow::saveAs);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("Close the document"));
    fileMenu->addAction(closeAct);
    fileToolBar->addAction(closeAct);

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    // Edit menu actions
#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
    QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, &QAction::triggered, textEdit, &QPlainTextEdit::cut);
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    menuBar()->addSeparator();

#endif // !QT_NO_CLIPBOARD

    // View menu actions
    const QIcon zoomOutIcon = QIcon::fromTheme("view-zoom-out", QIcon(":/images/zoom-out.png"));
    QAction *zoomOutAct = new QAction(zoomOutIcon, tr("&Zoom Out"), this);
    zoomOutAct->setShortcuts(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Zoom In"));
    //connect(zoomOutAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    viewMenu->addAction(zoomOutAct);
    viewToolBar->addAction(zoomOutAct);

    const QIcon zoomInIcon = QIcon::fromTheme("view-zoom-in", QIcon(":/images/zoom-in.png"));
    QAction *zoomInAct = new QAction(zoomInIcon, tr("&Zoom In"), this);
    zoomInAct->setShortcuts(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Zoom In"));
    //connect(zoomInAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    viewMenu->addAction(zoomInAct);
    viewToolBar->addAction(zoomInAct);

    const QIcon zoomResetIcon = QIcon::fromTheme("view-zoom-reset", QIcon(":/images/zoom-reset.png"));
    QAction *zoomResetAct = new QAction(zoomResetIcon, tr("&Zoom Reset"), this);
    zoomResetAct->setStatusTip(tr("Zoom Reset"));
    //connect(zoomResetAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    viewMenu->addAction(zoomResetAct);
    viewToolBar->addAction(zoomResetAct);

    menuBar()->addSeparator();

    // Navigation tool bar actions
    const QIcon pageDownIcon = QIcon::fromTheme("page-down", QIcon(":/images/page-down.png"));
    QAction *pageDownAct = new QAction(pageDownIcon, tr("&Previous Page"), this);
    pageDownAct->setStatusTip(tr("Previous Page"));
    //connect(pageDownAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    navToolBar->addAction(pageDownAct);

    const QIcon pageUpIcon = QIcon::fromTheme("page-up", QIcon(":/images/page-up.png"));
    QAction *pageUpAct = new QAction(pageUpIcon, tr("&Next Page"), this);
    pageUpAct->setStatusTip(tr("Next Page"));
    //connect(pageUpAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    navToolBar->addAction(pageUpAct);

    menuBar()->addSeparator();

    // Help menu actions
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

#ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
    connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

bool MainWindow::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = currentFile;
    if (currentFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

