#pragma once
#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr, QString extensionName = "txt");

protected:
	void highlightBlock(const QString &text) override;

private:
	struct HighlightingRule
	{
		QRegularExpression pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QRegularExpression commentStartExpression;
	QRegularExpression commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat classFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat functionFormat;
	QString highLightType;

};

#endif // HIGHLIGHTER_H
