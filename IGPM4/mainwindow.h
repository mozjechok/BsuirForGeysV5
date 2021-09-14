#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QValidator>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <string>
#include <set>
#include <algorithm>

using namespace QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void computeButton();

private:
    Ui::MainWindow *ui;

    const int a = 4, b = 8, m = 30;
    const double e = 0.0001;

    std::set<double> results;
};
#endif // MAINWINDOW_H
