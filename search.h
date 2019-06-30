#ifndef SEARCH_H
#define SEARCH_H

#include <QtWidgets>

namespace Ui {
class Search;
}

class Search :public QDialog
{
    Q_OBJECT
public:
    Search(bool isreplace, QWidget *parent = nullptr);
    bool searching;

private:
    Ui::Search *ui;


signals:
    void findNext(const QString &, Qt::CaseSensitivity, bool);
    void replaceNext(const QString &, const QString &, Qt::CaseSensitivity);
    void replaceAll (const QString &, const QString &, Qt::CaseSensitivity);
    void searchModeChange(int);

private slots:
    void on_buttonBox_accepted();
    void catchModeChange(int);
};

#endif // SEARCH_H
