#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

template<class T>
std::vector<T> generateX(int a, int b, int n)
{
    std::vector<T> x(n);

    for (int i = 0; i < x.size(); i++)
        x[i] = a + i * (b - a) / static_cast<T>(n - 1);

    return x;
}
template<class T, class F>
std::vector<T> calculateY(const std::vector<T> &x, F func)
{
    std::vector<T> y(x.size());

    for (int i = 0; i < x.size(); i++)
        y[i] = func(x[i]);

    return y;
}

template<class T>
T calculateFi(T x, const std::vector<T>& c)
{
    T powX = 1;
    T res = 0;

    for (int i = 0; i < c.size(); i++)
    {
        res += powX * c[i];
        powX *= x;
    }

    return res;
}

template<class T>
std::vector<T> calculateC(const std::vector<T> &x, const std::vector<T> &y)
{
    T d;
    int n;
    std::vector<T> c( x.size() );
    std::vector< std::vector<T> > a(x.size());
    for (auto& vec : a)
        vec.resize(x.size());

    for (int i = 0; i < c.size(); i++)
    {
        a[i][0] = 1;
        n = -1;

        for (int j = 1; j < c.size(); j++)
        {
            n++;
            if (n == i)
                n++;

            d = x[i] - x[n];
            a[i][j] = a[i][j - 1] / d;

            for (int j1 = j - 1; j1 >= 1; j1--)
                a[i][j1] = (a[i][j1 - 1] - a[i][j1] * x[n]) / d;

            a[i][0] = -a[i][0] * x[n] / d;
        }
    }

    for (int i = 0; i < c.size(); i++)
    {
        c[i] = 0;
        for (int k = 0; k < c.size(); k++)
            c[i] += a[k][i] * y[k];
    }

    return c;
}

template<class T>
std::vector<T> calculateDelta(const std::vector<T>& f1, const std::vector<T>& f2)
{
    std::vector<T> delta(f1.size());
    for (int i = 0; i < delta.size(); i++)
        delta[i] = f1[i] - f2[i];

    return delta;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto chart = new QChart();
    auto chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->centralwidget->layout()->addWidget(chartView);
    fillChart(chart);
}

void MainWindow::fillChart(QChart *chart)
{
    const int  a = 5, b = 8, n = 4;
    auto func = [](double x)
    {
        return sqrt(x) - pow(cos(x), 2);
    };

    auto x = generateX<double>(a, b, n);
    auto y = calculateY(x, func);
    auto c = calculateC(x, y);

    auto Fi = [c](double x)
    {
        return calculateFi(x, c);
    };

    auto x1 = generateX<double>(a, b, 21);
    auto func1 = calculateY(x1, func);
    auto Fi1 = calculateY(x1, Fi);
    auto delta = calculateDelta(func1, Fi1);

    auto series = new QLineSeries(chart);
    series->setName("f");
    for(int i = 0; i < x1.size(); i++)
    {
        series->append(x1[i], func1[i]);
    }
    chart->addSeries(series);

    series = new QLineSeries(chart);
    series->setName("fi");
    for(int i = 0; i < x1.size(); i++)
    {
        series->append(x1[i], Fi1[i]);
    }
    chart->addSeries(series);

    series = new QLineSeries(chart);
    series->setName("delta");
    for(int i = 0; i < x1.size(); i++)
    {
        series->append(x1[i], delta[i]);
    }
    chart->addSeries(series);

    chart->createDefaultAxes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

