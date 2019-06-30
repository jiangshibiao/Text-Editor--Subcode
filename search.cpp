#include "search.h"
#include "ui_search.h"
#include <QMessageBox>

Search::Search(bool isreplace, QWidget *parent)
    :QDialog(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(isreplace);
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(catchModeChange(int)));
    searching = false;
}

void Search::on_buttonBox_accepted(){

    int type = ui->tabWidget->currentIndex();
    if (type == 0){
        //查找功能
        if (ui->lineEdit1->text().isEmpty()){
            QMessageBox::warning(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("Searching text can't be empty!"), QMessageBox::Yes);
            this->show(); return;
        }
        Qt::CaseSensitivity Case = ui->checkBox1->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
        emit findNext(ui->lineEdit1->text(), Case, ui->checkBox2->isChecked());
    }
    else {
        //替换功能
        qDebug() << "Start replace";
        if (ui->lineEdit2->text().isEmpty()){
            QMessageBox::warning(this, tr("warning"), tr("Searching text can't be empty!"), QMessageBox::Yes);
            this->show(); return;
        }
        if (ui->lineEdit3->text().isEmpty()){
            QMessageBox::warning(this, tr("warning"), tr("Replacing text can't be empty!"), QMessageBox::Yes);
            this->show(); return;
        }
        Qt::CaseSensitivity Case = ui->checkBox3->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
        if (ui->checkBox4->isChecked())
            emit replaceAll (ui->lineEdit2->text(), ui->lineEdit3->text(), Case);
        else
            emit replaceNext(ui->lineEdit2->text(), ui->lineEdit3->text(), Case);
    }
}

void Search::catchModeChange(int index){
    emit searchModeChange(index);
}
