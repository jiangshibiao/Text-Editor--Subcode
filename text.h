#ifndef TEXT_H
#define TEXT_H

#include <QtWidgets>

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = nullptr);

public slots:
    void setCurrentFontSize(int);

private:

};
#endif // TEXT_H
