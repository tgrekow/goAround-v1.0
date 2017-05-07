#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QVector>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_fOpenBt_clicked();
    void on_ExtBt_clicked();
  //  void rawFltr(); 
  //  void on_listView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    QFile *myFile;
    QFileSystemModel *myFM;
    QVector<double> vTmp;
    QVector<QVector<double> > inData;
    QVector<QVector<double> > outData;

    void subFrame();
    void mFrame();
    void mean();
    void dev();
    void deltaRel();
    void dettaAbs();
    void diff1();
    void diff2();
    void qaqF();
    void fOut();
    void intf();
    void intd();
 //   void rFiltr();
};

#endif // MAINWINDOW_H
