#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

template<class T, class F>
T calculateD1F(T x, F func, T h)
{
    return (func(x + h) - func(x - h)) / (2 * h);
}

template<class T, class F>
T calculateD2F(T x, F func, T h)
{
    return (func(x + h) - 2 * func(x) + func(x - h)) / (h * h);
}

template<class T, class F>
T calculateMS(int a, int b, int m, F func)
{
    const double lezhandr = 0.5773502692;
    T s = 0;
    T h = (b - a) / (T)(m);
    T x = a + h / 2;
    for (int i = 0; i < m; i++)
    {
        s += func(x - h * lezhandr / 2 ) + func(x + h * lezhandr / 2);
        x += h;
    }

    return s * h / 2;
}

template<class T, class F1, class F2, class F3>
void calculate(int a, int b, T h, int m, F1 func, F2 derivedFunc1, F3 derivedFunc2,
    std::vector<T> &x, 
    std::vector<T> &y, 
    std::vector<T> &d1, 
    std::vector<T> &d2, 
    std::vector<T> &d1t, 
    std::vector<T> &d2t, 
    std::vector<T> &delta1, 
    std::vector<T> &delta2,
    T &MS)
{
    x.resize(21);
    y.resize(21);
    d1.resize(21);
    d2.resize(21);
    d1t.resize(21);
    d2t.resize(21);
    delta1.resize(21);
    delta2.resize(21);

    for (int i = 0; i < x.size(); i++)
    {
        x[i] = a + (i - 1) * (b - a) / static_cast<T>(x.size() - 1);
        y[i] = func(x[i]);

        d1[i] = calculateD1F(x[i], func, h);
        d2[i] = calculateD2F(x[i], func, h);

        d1t[i] = derivedFunc1(x[i]);
        d2t[i] = derivedFunc2(x[i]);

        delta1[i] = d1t[i] - d1[i];
        delta2[i] = d2t[i] - d2[i];
    }

    MS = calculateMS<double>(a, b, m, func);
}

template<class T>
void addSeriesToChart(QChart* chart, const std::vector<T> &vec1, const std::vector<T> &vec2, const char* seriesName)
{
    auto series = new QLineSeries();
    series->setName(seriesName);

    for (int i = 0; i < vec1.size(); i++)
        series->append(vec1[i], vec2[i]);

    chart->addSeries(series);
}

template<class T>
void fillCharts(QGridLayout* grid)
{
    const int  a = 5, b = 8;
    auto func = [](T x) -> T
    {
        return sqrt(x) - pow(cos(x), 2);
    };
    auto derivedFunc1 = [](T x) -> T
    {
        return 0.5 / sqrt(x) + sin(2 * x);
    };
    auto derivedFunc2 = [](T x) -> T
    {
        return -0.25 / pow(x, 1.5) + 2 * cos(2 * x);
    };

    const T     h[] = { 0.2, 0.1, 0.05 };
    const int   m[] = { 10, 20, 40 };

    std::vector<T> x;
    std::vector<T> y;
    std::vector<T> d1;
    std::vector<T> d2;
    std::vector<T> d1t;
    std::vector<T> d2t;
    std::vector<T> delta1;
    std::vector<T> delta2;
    T MS;

    for (int i = 0; i < 3; i++)
        grid->addWidget(new QLabel( QString("h = ") + std::to_string(h[i]).c_str() ), 2 * i + 3, 0, 2, 1, Qt::AlignCenter);

    for (int j = 0; j < 3; j++)
        grid->addWidget(new QLabel( QString("m = ") + std::to_string(m[j]).c_str() ), 0, j + 1, 1, 1, Qt::AlignCenter);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            auto chart = new QChart();
            calculate(a, b, h[i], m[j], func, derivedFunc1, derivedFunc2,
                x, y, d1, d2, d1t, d2t, delta1, delta2, MS);

            addSeriesToChart(chart, x, y, "y");
            addSeriesToChart(chart, x, d1, "d1");
            addSeriesToChart(chart, x, d2, "d2");
            addSeriesToChart(chart, x, d1t, "d1t");
            addSeriesToChart(chart, x, d2t, "d2t");
            addSeriesToChart(chart, x, delta1, "delta1");
            addSeriesToChart(chart, x, delta2, "delta2");

            chart->createDefaultAxes();
            auto chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);

            grid->addWidget(new QLabel(QString("MS = ") + std::to_string(MS).c_str()), 2 * i + 3, j + 1, Qt::AlignCenter);
            grid->addWidget(chartView, 2 * i + 4, j + 1);
        }

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillCharts<double>(ui->gridLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

