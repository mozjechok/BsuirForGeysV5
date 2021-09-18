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
#include <iostream>
#include <iomanip>

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

    template<class T>
    void fillDeltaChart();


protected slots:
    void nxChanged(const QString& nxText);

private:
    Ui::MainWindow *ui;

    const int a = 2, b = 4, m = 30;
    const double e = 0.0001;
};
#endif // MAINWINDOW_H
