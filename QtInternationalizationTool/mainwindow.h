#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_export_clicked();

    void on_pushButtoncsv_clicked();

private:
    Ui::MainWindow *ui;

    int  allLine_;
    int  passLine_;
    int  allFunc_;
    int  passFunc_;
    void makeContent(QString &content);
    QMap<QString, QString> getConfigContent();
};

#endif // MAINWINDOW_H
