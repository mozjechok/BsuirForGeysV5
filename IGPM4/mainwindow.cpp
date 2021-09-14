#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

template<class T, class F>
T calculateMG(T x0, T h, T e, int& it, F func)
{
    T x1 = x0 - h, x2 = x0, x3 = x0 + h;
    T y1 = func(x1), y2 = func(x2), y3 = func(x3);
    T z1, z2, r, d, p, q, D, zm1, zm2, zm;

    it = 0;
    do
    {
        it++;

        z1 = x1 - x3;
        z2 = x2 - x3;

        r = y3;
        d = z1 * z2 * (z1 - z2);
        p = ((y1 - y3) * z2 - (y2 - y3) * z1) / d;
        q = -((y1 - y3) * z2 * z2 - (y2 - y3) * z1 * z1) / d;

        if (q * q - 4 * p * r < 0)
            throw std::exception();

        D = sqrt(q * q - 4 * p * r);
        zm1 = (-q + D) / (2 * p);
        zm2 = (-q - D) / (2 * p);
        
        if (abs(zm1) > abs(zm2))
        {
            zm = zm2;
        }
        else
        {
            zm = zm1;
        }

        x1 = x2;
        x2 = x3;
        y1 = y2;
        y2 = y3;

        x3 = x3 + zm;
        y3 = func(x3);

    } while (abs(zm) >= e && it <= 100);

    return x3;
}

template<class T>
T calculateH(int a, int b, int m)
{
    return (b - a) / static_cast<T>(m);
}

template<class T, class F>
void fillChart(QChartView* chartView, F func, int a, int b, int m)
{
    auto chart = new QChart();
    auto series = new QLineSeries();

    T h = calculateH<double>(a, b, m);
    for(T x = a; x <= b; x+=h)
        series->append(x, func(x));

    chart->addSeries(series);
    chart->createDefaultAxes();

    chartView->setChart(chart);
}

double func(double x)
{
    return sqrt(x) - pow(cos(x), 2) - 2;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fillChart<double>(ui->chartView, func, a, b, m);
    connect(ui->computeButton, &QToolButton::pressed, this, &MainWindow::computeButton);

    auto validator = new QDoubleValidator(a, b, 10);
    QLocale locale(QLocale::English);
    validator->setLocale(locale);
    ui->x0Text->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::computeButton()
{
    try
    {
        QString x0Text = ui->x0Text->text();
        ui->x0Text->setText("");

        int it;
        double z = calculateMG(x0Text.toDouble(), calculateH<double>(a, b, m), e, it, func);

        QString resultText;
        results.insert(z);
        for (auto z : results)
            resultText.append(std::to_string(z).c_str() + QString("; "));

        ui->itText->setText(std::to_string(it).c_str());
        ui->resultText->setText(resultText);
    }
    catch (const std::exception& /*ex*/)
    {
        ui->x0Text->setPlaceholderText( QString::fromStdWString(L"Парабола в данной точке не имеет пересечений с осью Ox") );
    }
}

