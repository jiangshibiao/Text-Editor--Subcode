#include "text.h"

TextEdit::TextEdit(QWidget *parent)
    :QTextEdit(parent)
{}
void TextEdit::setCurrentFontSize(int size)
{
    setFontPointSize(size);
}
