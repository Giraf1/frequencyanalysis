#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QRegularExpression>
#include <QVector>
#include <string>
#include <QChar>
#include <QPair>
#include <QMessageBox>
#include <stdlib.h>
#include <QtAlgorithms>
#include <algorithm>
#include <QTextCursor>
#include <QKeyEvent>


#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>



MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QRegularExpression re("[0-9]|\\s");

QVector <int> numbers = {};
QList<QPair<int, int>> list;
QVector <QPair<int, QString>> replacment_for_numbers = {};
QString edited_input = {};
int max_count_numbers = 0;



void MainWindow::on_pushButton_clicked()
{
    list.clear();
    numbers.clear();
    replacment_for_numbers.clear();
    ui->lineEdit->clear();
    ui->textEdit_3->clear();

    edited_input.clear();

    if (ui->textEdit->toPlainText().size() == 0) {
        QMessageBox::critical(this, tr("Миссия окончательно провалена!"), tr("Вы не ввели значения, активировано самоуничтожение 3..2..1..."));
        exit(0);
    }
    else if (ui->textEdit->toPlainText()[0] == ' ') {
        QMessageBox::warning(this, tr("Миссия успешно провалена!"), tr("Вы ввели пробел первым символом, подумайте ещё раз."));
        exit(0);
    }
    else {
        auto input = ui->textEdit->toPlainText();
        for (int i = 0; i < input.size(); i++) {
            if(re.match(input[i]).hasMatch() &&
                    input[i] != '\n')

            edited_input.push_back(ui->textEdit->toPlainText()[i]);
        }
    }
    while(edited_input.contains("   ")) edited_input.replace(edited_input.indexOf("   "), 3, "  ");


    edited_input.push_front(" ");
    edited_input.push_back(" ");
    qDebug() << "edited_input:"<< edited_input;

    for (int i = 0; i < edited_input.size() - 2; i++) {
        if (edited_input[i] == ' ') {
            QString temp = {};
            for (int j = i; edited_input[j + 1] != ' '; j++) {
                temp.append(edited_input[j + 1]);
            }
            numbers.append(temp.toInt());
        }
    } // normalyok
    qDebug() << "numbers:"<< numbers;


    for (int i = 0 ; i < numbers.size(); i++)
    {
        if(list.isEmpty()) list.append(qMakePair(numbers[i], 0)); // kostul
        int temp = 0;
        int j = 0;
        for (;j < list.size(); j++) if(numbers[i] != list[j].first) temp++;
        if(temp == list.size()) list.append(qMakePair(numbers[i], 1));
        else
        {
            for (int z = 0; z < list.size(); z++) {
                if (list[z].first == numbers[i])
                    while (temp++ < list.size()) list[z].second++;
            }
        }
    }

    std::sort(list.begin(), list.end());
    qDebug() << "list:" << list;
    list.remove(0, 1);
    for (int i = 0; i < list.size(); i++) {
        ui->textEdit_3->append(QString::number(list[i].first) + " - " + QString::number(list[i].second));
        list[i].second > max_count_numbers ? max_count_numbers = list[i].second : max_count_numbers;
    }
    qDebug() << "max count of numbers:" << max_count_numbers;

    for (int i = 0; i < list.size(); i++) {
        replacment_for_numbers.append(qMakePair(list[i].first, "NULL"));
    }
    ui->comboBox->clear();
    for (int i = 0; i < list.size(); i++) {
        ui->comboBox->addItem(QString::number(list[i].first));
    }
    ui->pushButton_4->setEnabled(true);
    ui->textEdit_3->setEnabled(1);
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (ui->comboBox->currentIndex() == ui->comboBox->count()) {
        ui->label_4->setText("Ви вже додали заміни для усіх об'єктів");
        return;
    }
    ui->label_4->setText("У наступному рядку введіть заміну для " + arg1);
    if (replacment_for_numbers[ui->comboBox->currentIndex()].second != "NULL") {
        ui->lineEdit->setText(replacment_for_numbers[ui->comboBox->currentIndex()].second);
    }
    else {
        ui->lineEdit->clear();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if (ui->comboBox->currentIndex() == ui->comboBox->count()) {
        ui->lineEdit->setText("Ви вже додали заміни для усіх об'єктів");
        return;
    }
    qDebug() << "from combobox" << ui->comboBox->currentIndex();
    replacment_for_numbers[ui->comboBox->currentIndex()].second = ui->lineEdit->text();
    for (int i = 0; i < replacment_for_numbers.size(); i++) {
        qDebug() << replacment_for_numbers[i].first << replacment_for_numbers[i].second;
    }

    ui->comboBox->setCurrentIndex(ui->comboBox->currentIndex() + 1);

}


void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << "editet input" << edited_input;
    QString output = {};
    QVector <int> localnumbers = {};
    for (auto i : replacment_for_numbers) localnumbers.append(i.first);

    for (int i = 0; i < numbers.size() - 1; i++){
        qDebug() << "iter" << i;
        if (numbers[i] == 0) {
            output.append(' ');
            qDebug() << "nb" << numbers[i] << "true";
        }
        else {
            output.append(replacment_for_numbers[localnumbers.indexOf(numbers[i])].second);
            qDebug() << "nb" << numbers[i] << "false";
        }
    }
    ui->lineEdit->clear();
    ui->lineEdit->setText(output);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "key ivent" << event->key();
    if(event->key() == Qt::Key_Enter) {
        ui->comboBox->setCurrentIndex(ui->comboBox->currentIndex() + 1);
    }
}


void MainWindow::on_textEdit_2_textChanged()
{
//    QString input = ui->textEdit_2->toPlainText();
//    if (replacment_for_numbers.size() == 0) return;
//    if(input[input.size() - 1] == '\n') {

//    }
}

#include <QValueAxis>
#include <QPixmap>
#include <QClipboard>


void MainWindow::on_pushButton_4_clicked()
{


    QDialog *Dblack = new QDialog();
    Dblack->setMinimumSize(30*list.size(), 500);
    QVBoxLayout *vlayout = new QVBoxLayout;




    QChart *chart = new QChart();
    QBarSeries *series = new QBarSeries();
    series->setLabelsVisible(true);
    series->setBarWidth(0.8);
    QStringList categories;
    QBarSet *set = new QBarSet("");
    for (int i = 0; i < list.size(); i++) {
        *set << list[i].second;
        categories.append(QString::number(list[i].first));
    }
    series->append(set);
    chart->addSeries(series);

    chart->setAnimationOptions(QChart::SeriesAnimations);
    QBarCategoryAxis *Xaxis = new QBarCategoryAxis();
    QValueAxis *Yaxis = new QValueAxis;
    Xaxis->append(categories);
    Yaxis->setLabelFormat("%.0f");
    Yaxis->setMax(max_count_numbers + 3);
    chart->setAxisX(Xaxis, series);// Установить ось координат
    chart->setAxisY(Yaxis, series);
    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    vlayout->addWidget(chartView);

    QAbstractButton *bCopy = new QPushButton("copy");
    QAbstractButton *bExit = new QPushButton("back");
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(bCopy);
    hlayout->addWidget(bExit);
    vlayout->addLayout(hlayout);

    Dblack->setLayout(vlayout);
    Dblack->setWindowTitle("Графичок");
    Dblack->show();
    QClipboard *clipboard = QApplication::clipboard();


    Dblack->connect(bCopy, SIGNAL(clicked()), Dblack, [Dblack](QClipboard *clipboard) {
        clipboard->setText("1");
    }
    );
    Dblack->connect(bExit,SIGNAL(clicked()),Dblack,SLOT(close()));
}
/*
void MainWindow::copy(QChart qchart)
{

    clipboard->setPixmap(QPixmap());
}
*/

