#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

template<class T, class F, class F1>
void calculateM5(T a, T b, int nx, int np, std::vector<T> &y, F FPR, F1 OUT)
{
    std::vector<T> fp1(y.size()), fp2(y.size()), f1(y.size()), f2(y.size()),
        yp1(y.size()), yp2(y.size()), y1(y.size());

    T h = (b - a) / nx;
    T x = a;
    int i;

    if (np == 0)
        np = nx + 1;

    OUT(x, y);

    for (int n = 0; n < nx; n++)
    {
        FPR(x, y, fp1);
        for (i = 0; i < y.size(); i++)
            yp1[i] = y[i] + h * fp1[i] / 2;

        FPR(x + h / 2, yp1, fp2);
        for (i = 0; i < y.size(); i++)
            yp2[i] = y[i] + h * fp2[i] / 2;

        FPR(x + h / 2, yp2, f1);
        for (i = 0; i < y.size(); i++)
            y1[i] = y[i] + h * f1[i];

        FPR(x + h, y1, f2);
        for (i = 0; i < y.size(); i++)
            y[i] += h * (fp1[i] + 2 * fp2[i] + 2 * f1[i] + f2[i]) / 6;

        x += h;

        if (n % np == 1)
            OUT(x, y);
    }

    std::cout << "-------------------------------------------------------" << std::endl;
}

template<class T>
std::vector<T> FPR(T x, const std::vector<T> &y, std::vector<T> &F)
{
    F[0] = 2 * y[0] + (y[1] + exp(x)) / exp(x) - 4 * x;
    F[1] = 2 * x * y[1] / y[0];

    return F;
}

template<class T>
void OUT(T x, const std::vector<T> &y)
{
    std::cout << std::setprecision(4) << std::fixed
        << "x = " << x
        << " y1 = " << y[0]
        << " u1 = " << 2 * x
        << " d1 = " << 2 * x - y[0]
        << " y2 = " << y[1]
        << " u2 = " << exp(x)
        << " d2 = " << exp(x) - y[1]
        << std::endl;
}

template<class T>
void MainWindow::fillDeltaChart()
{
    auto series = new QLineSeries();
    auto chart = new QChart();
    ui->deltaChartView->setChart(chart);

    T d, dMax;
    T h;
    int i, nx = 10;
    std::vector<T> y(2), u(2);

    u[0] = 2 * b;
    u[1] = exp(static_cast<T>(b));

    do
    {
        h = (b - a) / nx;

        y[0] = 2 * a;
        y[1] = exp(static_cast<T>(a));

        calculateM5<T>(a, b, nx, 2, y, FPR<T>, OUT<T>);

        dMax = 0;
        for (i = 0; i < y.size(); i++)
        {
            d = abs(u[i] - y[i]);
            dMax = std::max(d, dMax);
        }

        series->append(nx, dMax);

        nx *= 2;
    } while (dMax > e);

    ui->hText->setText(std::to_string(nx / 2).c_str());
    ui->eText->setText(std::to_string(e).c_str());

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();
}

void MainWindow::nxChanged(const QString& nxText)
{
    int nx = nxText.toInt();
    auto y1Series = new QLineSeries();
    auto y2Series = new QLineSeries();
    auto u1Series = new QLineSeries();
    auto u2Series = new QLineSeries();
    std::vector<double> y(2);

    auto OUTToChart = [&](double x, const std::vector<double>& y) -> void
    {
        OUT(x, y);

        y1Series->append(x, y[0]);
        y2Series->append(x, y[1]);

        u1Series->append(x, 2 * x);
        u2Series->append(x, exp(x));
    };

    y[0] = 2 * a;
    y[1] = exp(static_cast<double>(a));

    calculateM5<double>(a, b, nx, 2, y, FPR<double>, OUTToChart);

    ui->yChartView->chart()->removeAllSeries();

    y1Series->setName("y1");
    y2Series->setName("y2");
    u1Series->setName("u1");
    u2Series->setName("u2");
    
    ui->yChartView->chart()->addSeries(y1Series);
    ui->yChartView->chart()->addSeries(y2Series);
    ui->yChartView->chart()->addSeries(u1Series);
    ui->yChartView->chart()->addSeries(u2Series);

    ui->yChartView->chart()->createDefaultAxes();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fillDeltaChart<double>();

    auto validator = new QIntValidator(10, 1000);
    ui->nxText->setValidator(validator);

    ui->yChartView->setChart(new QChart());

    connect(ui->nxText, &QLineEdit::textChanged, this, &MainWindow::nxChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}