/****************************************************************************************/
/*****                                                                              *****/
/*****  Data retrieval of raw or prim.retrieved (us-sub) file 06.05.2015 TD-Grekow  *****/
/*****   difference between raw/prim.retrved is an additional QAQ-func for raw sig  *****/
/*****                                                                              *****/
/****************************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

QString propDscrpt;
//bool rawKey;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    myFile = new QFile("");
    QString sPath="E:/Qt";
    myFM = new QFileSystemModel();
    myFM->setRootPath(sPath);
    myFM->setFilter(QDir::AllDirs|QDir::NoDot|QDir::Files);

    ui->listView->setModel(myFM);
    ui->listView->setRootIndex(myFM->setRootPath(sPath));
    ui->listView->setVisible(false);
}

MainWindow::~MainWindow(){    delete ui;}


void MainWindow::on_fOpenBt_clicked(){
    ui->listView->setEnabled(true);
    ui->listView->setVisible(true);
    ui->fOpenBt->setEnabled(false);
    ui->ExtBt->setEnabled(true);
}

void MainWindow::on_ExtBt_clicked(){
   exit(0);
}
void MainWindow::mFrame(){
    mean();// tworzy listę;
    dev(); // dalej każda funkcja uzupełnia listę;
    deltaRel();
    dettaAbs();
    diff1();
    diff2();
    intf();
    intd();
    if (ui->rawKey->isChecked()) qaqF();
}
void MainWindow::subFrame(){
    qDebug()<<"Not reDy yet!";
}
void MainWindow:: mean(){
    int Frame=ui->lineEdit->text().toInt();
    int Over=ui->lineEdit_2->text().toInt();
    int fLin = inData.count(); int cntr=0;
    double hlp=0;
    propDscrpt.clear();
    for(int i = 0; i<inData.at(1).count();i++)
        propDscrpt+="Mean"+QString::number(i+1)+" ";
    for (int i=0; i<(fLin-Frame); i+=(Frame-Over)){//iter; i++){
        vTmp.clear();
        for(int k = 0; k<inData.at(1).count();k++){
            for (int j =0; j<Frame;j++)
                hlp+=inData.at(j+i).at(k);
            hlp/=Frame;
            vTmp.append(hlp); hlp=0;

        }
        outData.append(vTmp);
        cntr++;qDebug()<<cntr<<":"<<vTmp;
    }
}

void MainWindow::dev(){
    for(int i = 0; i<inData.at(1).count();i++)
        propDscrpt+="Dev"+QString::number(i+1)+" ";
    int Frame=ui->lineEdit->text().toInt();
    int Over=ui->lineEdit_2->text().toInt();
    int fLin = inData.count();
    int iter=0;
    double hlp=0;
    for (int i=0; i<(fLin-Frame); i+=(Frame-Over)){
        vTmp.clear();
        vTmp=outData.at(iter);
        for(int k = 0; k<inData.at(1).count();k++){
            for (int j =0; j<Frame;j++)
                 hlp+=qPow((outData.at(iter).at(k)-inData.at(j+i).at(k)),2);
            hlp/=Frame;
            vTmp.append(sqrt(hlp)); hlp=0;
        }
        outData.replace(iter,vTmp);iter++;
    }
}
void MainWindow::deltaRel(){
    for(int i = 0; i<inData.at(1).count();i++)
        propDscrpt+="dRel"+QString::number(i+1)+" ";
    int Frame=ui->lineEdit->text().toInt();
    int Over=ui->lineEdit_2->text().toInt();
    int fLin = inData.count();
    int iter=0;
    double hlp=0;
    double min, max;
    for (int i=0; i<(fLin-Frame); i+=(Frame-Over)){
        vTmp.clear();
        vTmp=outData.at(iter);
        for(int k = 0; k<inData.at(1).count();k++){
            min = 1000; max=0;
            for (int j =0; j<Frame;j++){
                if(inData.at(j+i).at(k)>max) max=inData.at(j+i).at(k);
                if(inData.at(j+i).at(k)<min) min=inData.at(j+i).at(k);
             }
            hlp=max-min;
            vTmp.append(hlp);
        }
        outData.replace(iter,vTmp);iter++;
    }
}
void MainWindow::dettaAbs(){
    int column = inData.at(1).count();
    for(int i = 0; i<column;i++)
        propDscrpt+="dAbs_"+QString::number(i+1)+" ";//dopisz do pierwszej linii (opisu kolumn)
    int lines = outData.count();//ilość linii
    double a,b,hlp=0;
   // outData.replace(0,vTmp);
    for(int i =0; i<lines;i++){
        vTmp.clear();
        vTmp=outData.at(i);
        for(int j=0; j<column; j++){
            a=outData.at(i).at(j);
            b=outData.at(i).at(j+column*2);
            hlp=a + b/2;
            vTmp.append(hlp);
        }
        outData.replace(i,vTmp);
    }
}
void MainWindow::diff1(){
    int column = inData.at(1).count();
    for(int i = 0; i<column;i++)
        propDscrpt+="Df1_"+QString::number(i+1)+" ";//dopisz do pierwszej linii (opisu kolumn)
    int lines = outData.count();//ilość linii
    double a,b,hlp=0;
     vTmp=outData.at(0);
    for(int j=0; j<column; j++)
        vTmp.append(0); // bez Dff1
    outData.replace(0,vTmp);
    for(int i =1; i<lines;i++){
        vTmp.clear();
        vTmp=outData.at(i);
        for(int j=0; j<column; j++){
            a=outData.at(i).at(j);
            b=outData.at(i-1).at(j);
            hlp=a - b;
            vTmp.append(hlp);
        }
        outData.replace(i,vTmp);
    }
}
void MainWindow::diff2(){
    int column = inData.at(1).count();
    for(int i = 0; i<column;i++)
        propDscrpt+="Df2_"+QString::number(i+1)+" ";//dopisz do pierwszej linii (opisu kolumn)
    int lines = outData.count();//ilość linii
    double a,b,c,hlp=0;
    for (int i=0; i<2; i++){
        vTmp=outData.at(i);
        for(int j=0; j<column; j++)
            vTmp.append(hlp); // bez Dff1!
        outData.replace(i,vTmp);
    }
    for(int i =2; i<lines;i++){
        vTmp=outData.at(i);
        for(int j=0; j<column; j++){
            a=outData.at(i).at(j);
            b=outData.at(i-1).at(j);
            c=outData.at(i-2).at(j);
            hlp=(a - b)-(b-c);
            vTmp.append(hlp);
        }
        outData.replace(i,vTmp);
    }
}

void MainWindow::intf(){// sume of all values of the function
    int Frame=ui->lineEdit->text().toInt();
    int Over=ui->lineEdit_2->text().toInt();
    int fLin = inData.count();
    int cntr=0;
    double hlp=0;
    for(int i = 0; i<inData.at(1).count();i++)
        propDscrpt+="Int"+QString::number(i+1)+" ";
    for (int i=0; i<(fLin-Frame); i+=(Frame-Over)){
        vTmp=outData.at(cntr);
        for(int k = 0; k<inData.at(1).count();k++){
            for (int j =0; j<Frame;j++)
                hlp+=inData.at(j+i).at(k);
            vTmp.append(hlp); hlp=0;
        }
        outData.replace(cntr,vTmp);
        cntr++;
    }
}
void MainWindow::intd(){// sume of all diff of the function
    int Frame=ui->lineEdit->text().toInt();
    int Over=ui->lineEdit_2->text().toInt();
    int fLin = inData.count();
    double hlp=0;
    double nVal, old=0;
    int cntr=0;
    for(int i = 0; i<inData.at(1).count();i++)
        propDscrpt+="Idf"+QString::number(i+1)+" ";
    for (int i=0; i<(fLin-Frame); i+=(Frame-Over)){//iter; i++){
        vTmp=outData.at(cntr);
        for(int k = 0; k<inData.at(1).count();k++){
            for (int j =0; j<Frame;j++){
               nVal= inData.at(j+i).at(k)-old;
               hlp+=nVal;
               old=inData.at(j+i).at(k);
            }
            vTmp.append(hlp); hlp=0;
        }
        outData.replace(cntr,vTmp);
        cntr++;
    }
}
void MainWindow::qaqF(){
    for(int i = 0; i<inData.at(1).count();i++)
        propDscrpt+="Qq"+QString::number(i+1)+" ";
    int Frame=ui->lineEdit->text().toInt();
    int Over=ui->lineEdit_2->text().toInt();
    int fLin = inData.count();
    double hlp=0;
    int cntr=0;
    for (int i=0; i<(fLin-Frame); i+=(Frame-Over)){
        vTmp=outData.at(cntr);
        for(int k = 0; k<inData.at(1).count();k++){
            for (int j =0; j<Frame;j++)
                 if(inData.at(j+i).at(k)<0) hlp++;
            vTmp.append((hlp)); hlp=0;
        }
        outData.replace(cntr,vTmp); cntr++;
    }
}

void MainWindow::fOut(){
    QTextStream fLine(myFile);
    fLine<<propDscrpt<<endl;
    for(int i=0; i<outData.count();i++){
        for(int j=0; j<outData.at(1).count();j++)
            fLine<<QString::number(outData.at(i).at(j))<<" ";
        fLine<<endl;
    }
    myFile->close();
}
/*void MainWindow::rawFltr(){
    rawKey=true;
    ui->listView->setEnabled(true);
    ui->listView->setVisible(true);
    ui->fOpenBt->setEnabled(false);
    ui->fSaveBt->setEnabled(true);
}
void MainWindow::rFiltr(){//!!! po filtrowaniu odrazu robimy SUB - standard.versje!!!
    for(int i = 0; i<inData.at(1).count();i++)
        propDscrpt+="Qq"+QString::number(i+1)+" ";
    int Frame=ui->lineEdit->text().toInt();
    int Over=ui->lineEdit_2->text().toInt();
    int fLin = inData.count();
    double hlp=0;
    for (int i=0; i<(fLin-Frame); i+=(Frame-Over)){
        vTmp.clear();
        for(int k = 0; k<inData.at(1).count();k++){
            for (int j =0; j<Frame;j++)
                 if(inData.at(j+i).at(k)<0) hlp++;
            vTmp.append((hlp)); hlp=0;
        }
        outData.append(vTmp);
    }
}*/

void MainWindow::on_listView_doubleClicked(const QModelIndex &index){
    QString sPath = myFM->fileInfo(index).absoluteFilePath();

    QString  hlp1, fName=myFM->rootPath()+"/";
//qDebug()<<"absolute Path:"<<sPath;
//qDebug()<<"File Name:"<<fName<<" fName size:"<<fName.size()<<"Count:"<<fName.count();
//qDebug()<<"Diff:"<<sPath.right(sPath.count()-fName.count());
    ui->listView->setRootIndex(myFM->setRootPath(sPath));
    QModelIndex index1 = ui->listView->currentIndex();
    //common
    if(myFM->fileInfo(index1).isFile()){
       myFile->setFileName(myFM->fileInfo(index1).absoluteFilePath());
       if(!myFile->open(QFile::ReadOnly)) qDebug()<<"File not open!";
       QTextStream fLine(myFile);
       QString inLine,hlp,sTmp="";
       ui->listView->setEnabled(false);
       int i =0;
       inData.clear();
       outData.clear();
       vTmp.clear();
       while (!fLine.atEnd())    {
         inLine = fLine.readLine();
         hlp=inLine.mid(i,1);
         if (hlp>"9"){
             inLine = fLine.readLine();
             hlp=inLine.mid(i,1);
         }
         while (i<inLine.length()) {
             while((hlp!=" ")&&(hlp!=",")&&(i<inLine.length())){
                 sTmp+=hlp;
                 i++;
                  hlp=inLine.mid(i,1);
             }
             vTmp.append(sTmp.toDouble());sTmp.clear();
             while((hlp==" ")||(hlp==","))    {i++; hlp=inLine.mid(i,1);}
         }
         i=0;
         inData.append(vTmp);
         vTmp.clear();
       }
       myFile->close();
       ui->fOpenBt->setEnabled(false);
       //ze starego SAVE
       if(ui->outFname->isChecked()){
           QString a,b,c;
           //create a -> DIR!
           if(ui->rawKey->isChecked())a="r";else a="s";
           b=a+ui->lineEdit->text()+"_"+ui->lineEdit_2->text()+"/"+a;//+INfileName!!!
           c=sPath.right(sPath.count()-fName.count());
           fName+=b+c;
           qDebug()<<"a:"<<a<<" b:"<<b<<" c:"<<c<<endl<<"sPath:"<<sPath<<endl<<"fName:"<<fName;
       }
       else{
       hlp1=QInputDialog::getText(this,"OUTPUT FILE", "Enter File Name:");
       fName+=hlp1+".txt";}
       myFile->setFileName(fName);
       if(! myFile->open(QFile::WriteOnly))   qDebug()<<"File not open!";
       else{
         if(ui->checkBox->isChecked())  subFrame();//praca na subFrames
         else   mFrame();// praca na Frame - bez podzielenia się go
         fOut();
         ui->listView->setVisible(false);
         ui->fOpenBt->setEnabled(true);
        }
    }
}

//void MainWindow::on_listView_clicked(const QModelIndex &index) {}
