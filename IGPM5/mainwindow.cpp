#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

template<class T, class F, class F1>
T calculateMP3(T x0, T h, T e, int& it, F func, F1 derievedFunc)
{
    T x1 = x0, x2;
    T y1, y2;
    T z1, r, d1, d2, q, p, zm, sqr;

    d1 = derievedFunc(x1);
    if (d1 > 0)
        h *= -1;

    x2 = x1 + h;
    d2 = derievedFunc(x2);

    if ((d2 - d1) / h < 0)
        throw std::exception();

    y1 = func(x1);
    y2 = func(x2);

    it = 0;
    do
    {
        it++;

        z1 = x1 - x2;
        p = (d1 - d2 - 2.0 * (y1 - y2 - d2 * z1) / z1) / (z1 * z1);
        q = (d2 - d1 + 3.0 * (y1 - y2 - d2 * z1) / z1) / z1;

        r = d2;

        sqr = q * q - 3.0 * p * r;
        if (sqr < 0)
        {
            zm = -q / (3.0 * p);
        }
        else
        {
            zm = (-q + sqrt(sqr)) / (3.0 * p);
        }

        x1 = x2;
        y1 = y2;
        d1 = d2;

        x2 += zm;
        y2 = func(x2);
        d2 = derievedFunc(x2);

    } while (abs(zm) >= e);

    return x2;
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

    T h = calculateH<long double>(a, b, m);
    for (T x = a; x <= b; x += h)
        series->append(x, func(x));

    chart->addSeries(series);
    chart->createDefaultAxes();

    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setChart(chart);
}

long double func(long double x)
{
    return sqrt(x) - cos(x);
}

long double derievedFunc(long double x)
{
    return 0.5 / sqrt(x) + sin(x);
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fillChart<long double>(ui->chartView, func, a, b, m);
    connect(ui->computeButton, &QToolButton::pressed, this, &MainWindow::computeButton);

    auto validator = new QDoubleValidator(a, b, 10);
    QLocale locale(QLocale::English);
    validator->setLocale(locale);
    ui->x0Text->setValidator(validator);
    ui->hText->setValidator(validator);
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
        QString hText = ui->hText->text();

        int it;
        long double z = calculateMP3(x0Text.toDouble(), hText.toDouble(), e, it, func, derievedFunc);

        if (results.find(z) == results.end())
        {
            auto series = new QScatterSeries();
            series->setMarkerShape(QScatterSeries::MarkerShape::MarkerShapeCircle);
            ui->chartView->chart()->addSeries(series);
            series->append(z, func(z));
            ui->chartView->chart()->createDefaultAxes();
        }

        QString resultText;
        results.insert(z);
        for (auto z : results)
            resultText.append(std::to_string(z).c_str() + QString("; "));

        ui->itText->setText(std::to_string(it).c_str());
        ui->resultText->setText(resultText);
    }
    catch (const std::exception& /*ex*/)
    {
        ui->itText->setText(QString::fromStdWString(L"Парабола в данной точке не имеет пересечений с осью Ox"));
    }
}

