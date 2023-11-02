#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lru.h"
#include <list>
#include <iostream>
#include <sstream>
#include <string>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->buttonGenerate, &QPushButton::clicked, this, &MainWindow::validInput);
}

void MainWindow::validInput()
{
    std::string strInput = ui->lineInput->text().toStdString();
    std::list<int> pageUsed;
    int page;

    std::istringstream iss(strInput);
    while (true)
    {
        if (iss >> page)
        {
            // 将要访问到的页插入
            pageUsed.push_back(page);
        }
        else if (!iss.eof())
        {
            // 读取到非法字符，忽略并且继续下一个
            iss.clear();
            iss.ignore(1);
        }
        else
        {
            // EOF
            break;
        }
    }

    processInput(pageUsed);
}

void MainWindow::processInput(const std::list<int>& pageUsed)
{
    bool flag = false;
    auto capacity = ui->lineCapacity->text().toInt(&flag);

    if (!flag || capacity < 0)
    {
        ui->labelNotice->setText("缓存容量不是有效数字，请重新输入！");
    }
    else if(pageUsed.size() == 0)
    {
        ui->labelNotice->setText("输入框中没有任何数字，请重新输入！");
    }
    else
    {
        ui->tableResult->clear();
        // 缺页次数，当前轮次
        int lackCount = 0;
        int currEpoch = 0;
        infoPack dataReceived;
        LRU prototype(capacity);
        QStringList headers;

        ui->tableResult->setRowCount(capacity + 1);                 // 有多少行取决于缓存容量，额外多一行用于存储缺页标记
        ui->tableResult->setColumnCount(pageUsed.size());           // 有多少列取决于读取次数

        for (auto elem : pageUsed)
        {
            int currItem = 0;
            dataReceived = prototype.usePage(elem);
            lackCount += dataReceived.lackFlag;
            for (auto page : dataReceived.cache)
            {
                QTableWidgetItem *item = new QTableWidgetItem(QString("%1").arg(page));
                ui->tableResult->setItem(currItem, currEpoch, item);
                ui->tableResult->item(currItem, currEpoch)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ++currItem;
            }

            headers << QString::number(elem);

            QFont fontStyle = ui->tableResult->horizontalHeader()->font();
            fontStyle.setBold(true);
            ui->tableResult->horizontalHeader()->setFont(fontStyle);
            if (dataReceived.lackFlag)
            {
                // ==========================================
                // =          缺页中断 & 页面置换标记         =
                // ==========================================
                QTableWidgetItem *item = new QTableWidgetItem(lackCount > capacity ? QString("缺页中断\n页面置换") : QString("缺页中断\n"));
                ui->tableResult->setItem(capacity, currEpoch, item);
                ui->tableResult->item(capacity, currEpoch)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                // ==========================================
                // =           字体居中 & 新页标记           =
                // ==========================================
                ui->tableResult->item(0, currEpoch)->setFont(fontStyle);
                ui->tableResult->item(0, currEpoch)->setBackground(Qt::gray);
                ui->tableResult->item(0, currEpoch)->setForeground(Qt::black);
                // ==========================================
                // =              表格宽度自适应             =
                // ==========================================
                ui->tableResult->verticalHeader()->setSectionResizeMode(capacity, QHeaderView::ResizeToContents);
            }
            ++currEpoch;
        }
        ui->tableResult->setHorizontalHeaderLabels(headers);
        ui->labelNotice->setText(QString("缺页次数：%1").arg(lackCount));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

