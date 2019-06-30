#include "subcode.h"
#include "text.h"
#include "search.h"
#include "ui_subcode.h"

SubCode::SubCode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SubCode)
{
    ui->setupUi(this); waiting = false;

    highLighter = new Highlighter(ui->textEdit->document(), "txt");

    ui->menu_E->addAction(ui->actionsearch);
    ui->menu_E->addAction(ui->actionreplace);
    ui->menu_V->addAction(ui->actionsmaller);
    ui->menu_V->addAction(ui->actionlarger);

    fontComboBox = new QFontComboBox(this);
    fontSizeSpinBox = new QSpinBox(this);
    ui->textEdit->setFont(QFont("Courier New",20));

    fontSizeSpinBox->setRange(1,200);
    fontSizeSpinBox->setValue(20);
    fontComboBox->setCurrentFont(QFont("Courier New",20));
    ui->mainToolBar->addWidget(fontComboBox);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(fontSizeSpinBox);
    ui->mainToolBar->setMovable(false);

    connect(ui->actionundo,    SIGNAL(triggered(bool)), ui->textEdit, SLOT(undo()));
    connect(ui->actionredo,    SIGNAL(triggered(bool)), ui->textEdit, SLOT(redo()));
    connect(ui->actionpaste,   SIGNAL(triggered(bool)), ui->textEdit, SLOT(paste()));
    connect(ui->actioncopy,    SIGNAL(triggered(bool)), ui->textEdit, SLOT(copy()));
    connect(ui->actioncut,     SIGNAL(triggered(bool)), ui->textEdit, SLOT(cut()));
    connect(ui->actionsearch,  SIGNAL(triggered(bool)), this, SLOT(startSearch()));
    connect(ui->actionreplace, SIGNAL(triggered(bool)), this, SLOT(startReplace()));

    connect(ui->actionWheat,   SIGNAL(triggered(bool)), this, SLOT(changeWheat()));
    connect(ui->actionPaleTurquoise,   SIGNAL(triggered(bool)), this, SLOT(changePaleTurquoise()));
    connect(ui->actionLavenderBlush,   SIGNAL(triggered(bool)), this, SLOT(changeLavenderBlush()));

    connect(ui->actionsmaller, SIGNAL(triggered()),  this, SLOT(Smaller()));
    connect(ui->actionlarger,  SIGNAL(triggered()),  this, SLOT(Bigger()));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(fileChange()));
    connect(fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(fontChange()));
    connect(fontComboBox, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(fontChange()));


    setWindowModified(false);

    curName = QString();
    setStatus(0);
    connect(ui->actionnewfile, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->actionaddfile, SIGNAL(triggered()), this, SLOT(addFile()));
    connect(ui->actionsave,    SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionsaveas,  SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actioncompile, SIGNAL(triggered()), this, SLOT(compile()));
    connect(ui->actionrun,     SIGNAL(triggered()), this, SLOT(run()));

    QFile file("://theme/default.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
}

SubCode::~SubCode()
{
    delete ui;
}

void SubCode::fileChange(){
    if (waiting == true) waiting = false;
    else{
        //setWindowModified(true);
        setStatus(1);
    }
}

void SubCode::fontChange(){
    QFont now = fontComboBox->currentFont();
    now.setPointSize(fontSizeSpinBox->value());
    ui->textEdit->setFont(now);
}

void SubCode::Smaller(){
    int size = fontSizeSpinBox->value() - 1;
    if (size < 1) size = 1;
    fontSizeSpinBox->setValue(size);
    fontChange();
}

void SubCode::Bigger(){
    /*QFont tmp = ui->textEdit->currentFont();
    qDebug() << tmp.pointSize();
    tmp.setPointSize(tmp.pointSize() + 1);
    ui->textEdit->setFont(tmp);*/
    int size = fontSizeSpinBox->value() + 1;
    if (size > 120) size = 120;
    fontSizeSpinBox->setValue(size);
    fontChange();
}

void SubCode::Highlight(QString curName){
    QFileInfo sub(curName);
    delete highLighter;
    highLighter = new Highlighter(ui->textEdit->document(), sub.suffix());
}

void SubCode::setStatus(bool modified){
    //qDebug() << (++debug) << ' ' << curName << ' ' <<addstar;
    QString title = curName.isEmpty() ? "Untitled" : QFileInfo(curName).fileName();
    title = title + "[*]";
    setWindowModified(modified);
    setWindowTitle(title);
}


bool SubCode::savetoFile(QString fileName){
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString content = ui->textEdit->toPlainText();
    for (auto itr = content.cbegin(); itr < content.cend(); itr++)
        if (*itr == '\n') out << endl; else out << *itr;
    QApplication::restoreOverrideCursor();
    Highlight(curName = fileName);
    setStatus(0); waiting = true;
    return true;
}


bool SubCode::openFile(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString content;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    content = in.readAll();
    ui->textEdit->setPlainText(content);
    QApplication::restoreOverrideCursor();

    Highlight(curName = fileName);
    setStatus(0); waiting = true;
    return true;
}

bool SubCode::checkSave(){
    if (isWindowModified())
    {
        int getMessage = QMessageBox::warning(this, tr("Subcode"),
            tr("The file has modified, do you want to save it?\n"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (getMessage == QMessageBox::Yes) return saveFile();
        if (getMessage == QMessageBox::Cancel) return false;
    }
    return true;
}


bool SubCode::newFile(){
    if (!checkSave()) return false;
    curName = QString();
    ui->textEdit->clear();

    Highlight("1.txt");
    setStatus(0);

    return true;
}

bool SubCode::addFile(){
    if (!checkSave()) return false;
    bool result = false;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open text"), ".", tr(FILETYPE));
    if (!fileName.isEmpty())
        result = openFile(fileName);
    return false;
}

bool SubCode::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save text"), ".", tr(FILETYPE));
    if (!fileName.isEmpty())
        return savetoFile(fileName);
    return false;
}

bool SubCode::saveFile(){
    bool result = curName.isEmpty() ? saveAs() : savetoFile(curName);
    setStatus(0);
    return result;
}

bool SubCode::compile(){
    if (!checkSave()){
        if (!saveFile()) return false;
    }
    QFileInfo file(curName);
    if (file.suffix() == "cpp"){
        QString cmd = "g++ " + file.filePath() + " -o " + file.baseName() + ".exe";
        qDebug() << cmd;
        if (system(cmd.toLatin1().data())){
            QMessageBox::critical(this, tr("Subcode"), tr("Compiling error!"));
            return false;
        }
        return true;
    }
    if (file.suffix() == "py"){
        QString cmd = "python " + file.filePath();
        if (system(cmd.toLatin1().data())){
            QMessageBox::critical(this, tr("Subcode"), tr("Compiling error!"));
            return false;
        }
        return true;
    }
    QMessageBox::critical(this, tr("Subcode"), tr("Only support C++ or Python"));
    return false;
}

bool SubCode::run(){
    if (!compile()) return false;
    QFileInfo file(curName);
    if (file.suffix() == "cpp"){
        QString cmd = file.baseName() + ".exe";
        if (system(cmd.toLatin1().data())){
            QMessageBox::critical(this, tr("Subcode"), tr("Running error!"));
            return false;
        }
    }
    return true;
}

void SubCode::searchModeChange(int index){
    search->setVisible(false);
    if (index == 0) startSearch(); else startReplace();
}

void SubCode::startSearch(){
    search = new Search(false);
    search->show();
    connect(search, SIGNAL(searchModeChange(int)), this, SLOT(searchModeChange(int)));
    connect(search, SIGNAL(findNext(const QString &, Qt::CaseSensitivity, bool)), this, SLOT(findNext(const QString &, Qt::CaseSensitivity, bool)));
}

void SubCode::findNext(const QString &str, Qt::CaseSensitivity cs, bool fromstart){

    if (fromstart && search->searching == false)
           ui->textEdit->moveCursor(QTextCursor::Start), search->searching = true;
    bool findSucc = cs == Qt::CaseInsensitive ? ui->textEdit->find(str)
                                              : ui->textEdit->find(str, QTextDocument::FindCaseSensitively);
    if (!findSucc){
        QMessageBox::information(this, tr("SubCode"), tr("Can't find \"%1\".").arg(str), QMessageBox::StandardButton::Ok);
        search->searching = false;
    }
    search->show();
}


void SubCode::startReplace(){
    //qDebug() << "Start replace2";
    search = new Search(true);
    search->show();
    connect(search, SIGNAL(searchModeChange(int)), this, SLOT(searchModeChange(int)));
    connect(search, SIGNAL(replaceNext(const QString &, const QString &, Qt::CaseSensitivity)),
              this, SLOT(replaceNext(const QString &, const QString &, Qt::CaseSensitivity)));
    connect(search, SIGNAL(replaceAll(const QString &, const QString &, Qt::CaseSensitivity)),
              this, SLOT(replaceAll(const QString &, const QString &, Qt::CaseSensitivity)));
}

bool SubCode::replaceNext(const QString &str1, const QString &str2, Qt::CaseSensitivity cs)
{
    bool findSucc = cs == Qt::CaseInsensitive ? ui->textEdit->find(str1)
                                              : ui->textEdit->find(str1, QTextDocument::FindCaseSensitively);
    if (!findSucc)
    {
        QMessageBox::information(this, tr("SubCode"), tr("Can't find \"%1\".").arg(str1), QMessageBox::StandardButton::Ok);
        search->show();
        return false;
    }
    ui->textEdit->textCursor().deleteChar();
    //qDebug()<<"delete";
    ui->textEdit->textCursor().insertText(str2);
    //qDebug()<<"insert";
    search->show();
    return true;
}

bool SubCode::replaceAll(const QString &str1, const QString &str2, Qt::CaseSensitivity cs)
{
    ui->textEdit->moveCursor(QTextCursor::Start);
    int replacenum = 0;
    while (true){
        bool findSucc = cs == Qt::CaseInsensitive ? ui->textEdit->find(str1)
                                                  : ui->textEdit->find(str1, QTextDocument::FindCaseSensitively);
        if (!findSucc) break;
        replacenum++;
        ui->textEdit->textCursor().deleteChar();
        ui->textEdit->textCursor().insertText(str2);
    }
    if (!replacenum)
    {
        QMessageBox::information(this, tr("SubCode"), tr("Can't find \"%1\".").arg(str1), QMessageBox::StandardButton::Ok);
        search->show(); return false;
    }
    QMessageBox::information(this, tr("SubCode"), tr("%1 texts have been replaced.").arg(replacenum), QMessageBox::StandardButton::Ok);
    search->show(); return true;
}

void SubCode::changeWheat(){
    QFile file("://theme/default.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
}

void SubCode::changePaleTurquoise(){
    QFile file("://theme/dark.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);

}

void SubCode::changeLavenderBlush(){
    QFile file("://theme/lavender.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
}
