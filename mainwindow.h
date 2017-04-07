#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "iocr.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(IOCR& ocrModule, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void processFile();
    void languageChanged(bool triggered);

private:
    IOCR& m_ocrModule;
    Ui::MainWindow* m_ui;

    void processImage(const QString& filepath);
};

#endif // MAINWINDOW_H
