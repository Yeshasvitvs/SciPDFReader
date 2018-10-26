#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolBar>
#include <QSettings>
#include <QApplication>
#include <QTextStream>
#include <QDesktopWidget>

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        MainWindow(QWidget * parent = nullptr);

        void loadFile(const QString &fileName);

protected:
        void closeEvent(QCloseEvent *event) override;

private slots:
        void newFile();
        void open();
        void about();
        void documentWasModified();

private:
        QMenu* fileMenu;
        QMenu* viewMenu;
        QMenu* windowMenu;
        QMenu* helpMenu;

        QToolBar* fileToolBar;
        QToolBar* viewToolBar;
        QToolBar* navToolBar;

        void createMenus();
        void createToolBars();
        void createActions();
        void createStatusBar();
        void readSettings();
        void writeSettings();
        bool maybeSave();
        bool saveFile(const QString &fileName);
        void setCurrentFile(const QString &fileName);
        QString strippedName(const QString &fullFileName);

        QPlainTextEdit *textEdit;
        QString currentFile;
};
