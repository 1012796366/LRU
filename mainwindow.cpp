#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <list>
#include <cstdlib>
#include "lru.h"
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
        // 缺页次数，当前轮次
        int lackCount = 0;
        int currEpoch = 0;
        infoPack dataReceived;
        LRU prototype(capacity);

        ui->tableResult->setRowCount(capacity);                 // 有多少行取决于缓存容量
        ui->tableResult->setColumnCount(pageUsed.size());       // 有多少列取决于读取次数

        for (auto elem : pageUsed)
        {
            int currItem = 0;
            dataReceived = prototype.usePage(elem);
            lackCount += dataReceived.lackFlag;
            for (auto page : dataReceived.cache)
            {
                QTableWidgetItem *item = new QTableWidgetItem(QString("%1").arg(page));
                ui->tableResult->setItem(currItem, currEpoch, item);
                if (page == elem && dataReceived.lackFlag)
                {
                    ui->tableResult->item(currItem, currEpoch)->setBackground(Qt::gray);
                    ui->tableResult->item(currItem, currEpoch)->setForeground(Qt::cyan);
                }
                ++currItem;
            }
            ++currEpoch;
        }

        ui->labelNotice->setText(QString("缺页次数：%1").arg(lackCount));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

