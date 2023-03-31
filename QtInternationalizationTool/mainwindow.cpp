#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGlobal>
#include <iostream>
#include <iomanip>
#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeContent(QString &content)
{
    ui->lineEdit->clear();
    QMap<QString, QString> dataMap = getConfigContent();    //获取Map列表


    QString orgContent = content;

    for (auto key : dataMap.keys())
    {
        //待替换内容, 用QRegularExpression代替 QRegExp, QRegExp不支持懒惰匹配等 https://blog.csdn.net/qq_43680827/article/details/128892857

        QRegularExpression oldStr = QRegularExpression("<translation( type=\"unfinished\")?>.*?</translation>");

        //替换后的内容
        QString newStr = QString("<translation type=\"unfinished\">%1</translation>").arg(dataMap[key]);

        if (ui->checkBox->isChecked())
        {
            newStr = QString("<translation>%1</translation>").arg(dataMap[key]);
        }

        int startFind = 0;
        while (1)
        {
            //寻找原语言标签
            int index = orgContent.indexOf(QString("<source>%1</source>").arg(key), startFind);
            if (index < 0)
            {
                break;
            }
            //更新位置到原语言标签内容之后
            startFind = QString("<source>%1</source>").arg(key).size() + index;

            QString matchedString = "";
            //更获取匹配值
            QRegularExpressionMatch match = oldStr.match(orgContent, startFind) ;
            if (match.hasMatch())
            {
                //获取匹配内容的终止位置,因为0 是代表整个匹配, >0的值代表匹配到的内容的每小部分
                int endPos = match.capturedEnd(0);
                //获取匹配内容的起始位置,因为0 是代表整个匹配, >0的值代表匹配到的内容的每小部分
                int startPos = match.capturedStart(0);
                //获取匹配到的整体内容
                matchedString = match .captured(0);
                //获取本段后的下一个段的开始位置
                startFind = endPos - matchedString.size() + newStr.size();
                //替换
                orgContent.replace(startPos, matchedString.size(), newStr);
            }
            else
            {
                ui->lineEdit->setText("ERROR");
                break;
            }
        }
    }
    content = orgContent;
}


QMap<QString, QString>  MainWindow::getConfigContent()
{
    QMap<QString, QString> dataMap;

    QFile file(ui->lineEditconfig->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return QMap<QString, QString> ();
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (fields.size() == 2)
        {
            dataMap.insert(fields[0], fields[1]);
        }
    }

    file.close();
    return dataMap;
}

void MainWindow::on_pushButton_clicked()
{
    QFile file(ui->lineEditname->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
    }

    // 读取文件内容
    QTextStream in(&file);
    QString content = in.readAll();
//    qDebug() << content;
    makeContent(content);
    // 新建 HTML 文件
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QFile outFile(ui->lineEditname->text());
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to create file";
    }

    // 写入替换后的内容
    QTextStream out(&outFile);
    out << content;

    // 关闭文件
    file.close();
    outFile.close();

}


void MainWindow::on_pushButton_export_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open DCM"),
                                                    "~/", "*.ts(*.ts)");
    if (!fileName.isEmpty())
    {
        ui->lineEditname->setText(fileName);
    }
}

void MainWindow::on_pushButtoncsv_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open DCM"),
                                                    "~/", "*.csv(*.CSV)");
    if (!fileName.isEmpty())
    {
        ui->lineEditconfig->setText(fileName);
    }
}
