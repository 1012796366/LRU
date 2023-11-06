#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lru.h"
#include <list>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>


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
    bool flag = false;
    int page;

    std::istringstream iss(strInput);
    while (true)
    {
        if (iss >> page)
        {
            // 将要访问到的页插入
            pageUsed.push_back(abs(page));
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

    // 尝试读取缓存容量
    auto capacity = ui->lineCapacity->text().toInt(&flag);
    if (!flag || capacity <= 0)
    {
        // 缓存容量没有有效数字
        ui->labelNotice->setText("缓存容量不是有效数字，请重新输入！");
    }
    else if (pageUsed.size() == 0)
    {
        // 缓存容量有效，但输入框没有有效数字
        ui->labelNotice->setText("输入框中没有任何数字，请重新输入！");
    }
    else
    {
        // 缓存容量和输入框都有有效数字
        processInput(pageUsed, capacity);
    }
}

void MainWindow::processInput(const std::list<int>& pageUsed, const int& capacity)
{
    ui->tableResult->clear();
    // 缺页次数，当前轮次
    int lackCount = 0;
    int currEpoch = 0;
    infoPack dataReceived;
    LRU prototype(capacity);
    QStringList headers;

    // 设置表格尺寸
    ui->tableResult->setRowCount(capacity + 1);                 // 有多少行取决于缓存容量，额外多一行用于存储缺页标记
    ui->tableResult->setColumnCount(pageUsed.size());           // 有多少列取决于读取次数

    for (auto elem : pageUsed)
    {
        QFont fontChosen = ui->tableResult->horizontalHeader()->font();
        QFont fontNormal = ui->tableResult->horizontalHeader()->font();
        fontChosen.setBold(true);
        fontNormal.setBold(false);
        ui->tableResult->horizontalHeader()->setFont(fontChosen);

        dataReceived = prototype.usePage(elem);
        lackCount += dataReceived.lackFlag;
        for (auto page : dataReceived.cache)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString("%1").arg(page.first));
            ui->tableResult->setItem(page.second, currEpoch, item);
            ui->tableResult->item(page.second, currEpoch)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            // 橘色被选中，浅蓝常规
            if (elem == page.first)
            {
                // 该页号被使用
                // 缺页 -> 橙色
                // 已有 -> 灰色
                ui->tableResult->item(page.second, currEpoch)->setBackground(dataReceived.lackFlag ? QColor(255, 130, 71) : Qt::gray);
                ui->tableResult->item(page.second, currEpoch)->setFont(fontChosen);
            }
            else
            {
                ui->tableResult->item(page.second, currEpoch)->setBackground(QColor(154, 245, 255));
            }

        }
        // 将页号用作为表格表头
        headers << QString::number(elem);

        if (dataReceived.lackFlag)
        {
            // ==========================================
            // =          缺页中断 & 页面置换标记         =
            // ==========================================
            QTableWidgetItem *item = new QTableWidgetItem(lackCount > capacity ? QString("缺页中断\n页面置换") : QString("缺页中断\n"));
            ui->tableResult->setItem(capacity, currEpoch, item);
            ui->tableResult->item(capacity, currEpoch)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
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

MainWindow::~MainWindow()
{
    delete ui;
}

