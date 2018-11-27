#include "MainWindow.h"
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    connect(m_ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(choosePng()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::choosePng()
{
//    const auto paths = QFileDialog::getOpenFileNames(this, "Please choose PNG", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "*.png");

    const QString dir = QFileDialog::getExistingDirectory(this,tr("Open Directory"),
                                                            "./");

    if(dir.isEmpty()) { return; }
    QDir path(dir);
    QFileInfoList fileList = path.entryInfoList(QStringList() << "*.png" << "*.gif" << ".jpg");
//    qDebug() << "fileList = " << fileList;

    foreach (QFileInfo file, fileList) {
        QString p = file.absoluteFilePath();
        QImage image(p);
        if(image.isNull()) { continue; }

        if(!image.save(p, "PNG"))
        {
            QMessageBox::warning(this, "Error", QString("%1 save fail").arg(p));
        }
        else{
            qDebug() << "success..";
        }
    }

    QMessageBox::information(this, "Done", "All png image processing is completed");

/*
    for(const auto &path: paths)
    {
        QImage image(path);
        if(image.isNull()) { continue; }

        if(!image.save(path, "PNG"))
        {
            QMessageBox::warning(this, "Error", QString("%1 save fail").arg(path));
        }
    }
    */

}
