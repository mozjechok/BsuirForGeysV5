#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QValidator>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QScatterSeries>

#include <string>
#include <unordered_set>
#include <algorithm>
#include <limits>

using namespace QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void computeButton();

private:
    Ui::MainWindow* ui;

    const int a = 4, b = 20, m = 80;
    const double e = std::numeric_limits<double>::epsilon();

    std::unordered_set<double> results;
};
#endif // MAINWINDOW_H
