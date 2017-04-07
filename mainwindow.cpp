#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>

MainWindow::MainWindow(IOCR& ocrModule, QWidget *parent) : QMainWindow{parent}, m_ocrModule{ocrModule}, m_ui{new Ui::MainWindow}
{
    m_ui->setupUi(this);
    connect(m_ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(processFile()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::processImage(const QString &filepath)
{
    const std::string recognizedText = m_ocrModule.processImage(filepath.toStdString(), nullptr);
    m_ui->RecognizedText->setText(QString::fromStdString(recognizedText));
}

void MainWindow::processFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty())
    {
        m_ui->statusBar->showMessage("Character recognition in progress...");
        processImage(fileName);
        m_ui->statusBar->showMessage("Done.");
    }
}
