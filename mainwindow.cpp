#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(IOCR& ocrModule, QWidget *parent) : QMainWindow{parent}, m_ocrModule{ocrModule}, m_ui{new Ui::MainWindow}
{
    m_ui->setupUi(this);
    connect(m_ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(processFile()));
    connect(m_ui->englishLangSet, SIGNAL(toggled(bool)), this, SLOT(languageChanged(bool)));
    connect(m_ui->polishLangSet, SIGNAL(toggled(bool)), this, SLOT(languageChanged(bool)));
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
    QString homeDirectory = QDir::homePath();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), homeDirectory, tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty())
    {
        m_ui->statusBar->showMessage("Character recognition in progress...");
        processImage(fileName);
        m_ui->statusBar->showMessage("Done.");
    }
}

void MainWindow::languageChanged(bool triggered)
{
    QObject* obj = sender();
    if (obj == m_ui->polishLangSet)
    {
        m_ui->englishLangSet->setChecked(!triggered);
        m_ocrModule.changeLanguage(triggered ? "pol" : "eng");
    }
    else if (obj == m_ui->englishLangSet)
    {
        m_ui->polishLangSet->setChecked(!triggered);
        m_ocrModule.changeLanguage(triggered ? "eng" : "pol");
    }
}
