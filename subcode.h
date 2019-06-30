#ifndef SUBCODE_H
#define SUBCODE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QTimer>
#include "highlighter.h"
#include "search.h"

#define FILETYPE "All files (*.*);;Text files (*.txt);;C++ source files (*.cpp);;Python source files (*.py)"

namespace Ui {
class SubCode;
}

class SubCode : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubCode(QWidget *parent = nullptr);
    ~SubCode();

private:
    Ui::SubCode *ui;
    QFontComboBox *fontComboBox;
    QSpinBox *fontSizeSpinBox;
    QString curName;
    Highlighter *highLighter;
    Search *search;

    bool waiting;
    bool savetoFile(QString fileName);       //保存至一个叫做 fileName 的文件
    bool openFile(QString fileName);         //打开一个叫做 fileName 的文件
    bool checkSave();                        //判断当前文件是否保存
    void setStatus(bool star);                //设置当前文件状态
    void Highlight(QString curName);

private slots:
    bool newFile();
    bool addFile();
    bool saveFile();
    bool saveAs();
    void fileChange();
    void fontChange();
    void Smaller();
    void Bigger();
    bool compile();
    bool run();

    void startSearch();
    void findNext(const QString &, Qt::CaseSensitivity, bool);

    void startReplace();
    bool replaceNext(const QString &, const QString &, Qt::CaseSensitivity);
    bool replaceAll (const QString &, const QString &, Qt::CaseSensitivity);

    void searchModeChange(int index);

    void changeWheat();
    void changePaleTurquoise();
    void changeLavenderBlush();

};

#endif // SUBCODE_H
