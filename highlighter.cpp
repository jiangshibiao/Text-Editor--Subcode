#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent, QString extensionName)
	: QSyntaxHighlighter(parent), highLightType(extensionName)
{
	if (highLightType == "cpp") /*c++�������*/
	{
		HighlightingRule rule;

		/*��ע�� ���⴦��*/
		multiLineCommentFormat.setForeground(Qt::red);
		commentStartExpression = QRegularExpression("/\\*");
		commentEndExpression = QRegularExpression("\\*/");

		/*����*/
		functionFormat.setFontItalic(true);
		functionFormat.setForeground(Qt::blue);
		rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
		rule.format = functionFormat;
		highlightingRules.append(rule);

		/*������ д����ȥ��*/
		keywordFormat.setForeground(Qt::darkBlue);
		keywordFormat.setFontWeight(QFont::Bold);
		QStringList keywordPatterns;
		keywordPatterns << "\\basm\\b" << "\\bauto\\b" << "\\bbool\\b"
			<< "\\bbreak\\b" << "\\case\\b" << "\\bcatch\\b"
			<< "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
			<< "\\bconst_cast\\b" << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdo\\b"
			<< "\\bdouble\\b" << "\\bdynamic_cast\\b" << "\\belse\\b" << "\\benum\\b" << "\\bexplicit\\b"
			<< "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b" << "\\bfloat\\b" << "\\bfor\\b"
			<< "\\bfriend\\b" << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b" << "\\bint\\b"
			<< "\\blong\\b" << "\\bmutable\\b" << "\\bnamespace\\b" << "\\bnew\\b" << "\\boperator\\b"
			<< "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" << "\\bregister\\b" << "\\breinterpret_cast\\b"
			<< "\\breturn\\b" << "\\bshort\\b" << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_cast\\b" 
			<< "\\bstruct\\b" << "\\bswitch\\b" << "\\btemplate\\b" << "\\bthis\\b"
			<< "\\bsignals\\b" << "\\bsigned\\b"
			<< "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
			<< "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
			<< "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
			<< "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b";
		foreach(const QString &pattern, keywordPatterns) {
			rule.pattern = QRegularExpression(pattern);
			rule.format = keywordFormat;
			highlightingRules.append(rule);
		}				
		
		/*Q����*/
		classFormat.setFontWeight(QFont::Bold);
		classFormat.setForeground(Qt::darkMagenta);
		rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
		rule.format = classFormat;
		highlightingRules.append(rule);

		/*�����ַ���*/
		quotationFormat.setForeground(Qt::darkGreen);
		rule.pattern = QRegularExpression("\".*\"");
		rule.format = quotationFormat;
		highlightingRules.append(rule);

		/*����ע��*/
		singleLineCommentFormat.setForeground(Qt::red);
		rule.pattern = QRegularExpression("//[^\n]*");
		rule.format = singleLineCommentFormat;
		highlightingRules.append(rule);
	}
	else if (highLightType == "py") /*python�������*/
	{
		HighlightingRule rule;

		/*��ע�� ���⴦��*/
		multiLineCommentFormat.setForeground(Qt::red);
		commentStartExpression = QRegularExpression("'''");
		commentEndExpression = QRegularExpression("'''");

		/*����*/
		functionFormat.setFontItalic(true);
		functionFormat.setForeground(Qt::blue);
		rule.pattern = QRegularExpression("\\b\\w+(?=\\()");
		rule.format = functionFormat;
		highlightingRules.append(rule);

		/*������*/
		keywordFormat.setForeground(Qt::darkBlue);
		keywordFormat.setFontWeight(QFont::Bold);
		QStringList keywordPatterns;
		keywordPatterns << "\\band\\b" << "\\bas\\b" << "\\bassert\\b"
			<< "\\bbreak\\b" << "\\bclass\\b" << "\\bcontinue\\b"
			<< "\\bdef\\b" << "\\belif\\b" << "\\belse\\b"
			<< "\\bexport\\b" << "\\bfinally\\b" << "\\bfor\\b"
			<< "\\bif\\b" << "\\bimport\\b" << "\\bin\\b"
			<< "\\bis\\b" << "\\blambda\\b" << "\\bnot\\b"
			<< "\\bor\\b" << "\\bpass\\b" << "\\braise\\b"
			<< "\\breturn\\b" << "\\btry\\b" << "\\bwhile\\b"
			<< "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b"
			<< "\\bTrue\\b" << "\\bFalse\\b" << "\\bNone\\b";
		foreach(const QString &pattern, keywordPatterns) {
			rule.pattern = QRegularExpression(pattern);
			rule.format = keywordFormat;
			highlightingRules.append(rule);
		}		

		/*�����ַ���*/
		quotationFormat.setForeground(Qt::darkGreen);
		rule.pattern = QRegularExpression("\".*\"");
		rule.format = quotationFormat;
		highlightingRules.append(rule);
		quotationFormat.setForeground(Qt::darkGreen);
		rule.pattern = QRegularExpression("'.*'");
		rule.format = quotationFormat;
		highlightingRules.append(rule);

		/*����ע��*/
		singleLineCommentFormat.setForeground(Qt::red);
		rule.pattern = QRegularExpression("#[^\n]*");
		rule.format = singleLineCommentFormat;
		highlightingRules.append(rule);
	}
	else /*�޸���*/
	{
	}
}

void Highlighter::highlightBlock(const QString &text)
{
	if (highLightType == "cpp") /*c++�������*/
	{
		/*��׼����*/
		foreach(const HighlightingRule &rule, highlightingRules) {
			QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
			while (matchIterator.hasNext()) {
				QRegularExpressionMatch match = matchIterator.next();
				setFormat(match.capturedStart(), match.capturedLength(), rule.format);
			}
		}
		/*��ע�� ���⴦��*/
		setCurrentBlockState(0);

		int startIndex = 0;
		if (previousBlockState() != 1)
			startIndex = text.indexOf(commentStartExpression);

		while (startIndex >= 0) {
			QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
			int endIndex = match.capturedStart();
			int commentLength = 0;
			if (endIndex == -1) {
				setCurrentBlockState(1);
				commentLength = text.length() - startIndex;
			}
			else {
				commentLength = endIndex - startIndex
					+ match.capturedLength();
			}
			setFormat(startIndex, commentLength, multiLineCommentFormat);
			startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
		}
	}
	else if (highLightType == "py") /*python�������*/
	{
		/*��׼����*/
		foreach(const HighlightingRule &rule, highlightingRules) {
			QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
			while (matchIterator.hasNext()) {
				QRegularExpressionMatch match = matchIterator.next();
				setFormat(match.capturedStart(), match.capturedLength(), rule.format);
			}
		}
		/*��ע�� ���⴦��*/
		setCurrentBlockState(0);

		int startIndex = -2;/*Σ�մ��룿*/
		if (previousBlockState() != 1)
			startIndex = text.indexOf(commentStartExpression);
		while (startIndex != -1) {
			QRegularExpressionMatch match;
			if (startIndex == -2) /*ע�ⲻ��ƥ�䵽����*/
			{
				startIndex = 0;
				match = commentEndExpression.match(text, startIndex);
			}
			else match = commentEndExpression.match(text, startIndex + 3);

			int endIndex = match.capturedStart();
			int commentLength = 0;
			if (endIndex == -1) {
				setCurrentBlockState(1);
				commentLength = text.length() - startIndex;
			}
			else {
				commentLength = endIndex - startIndex
					+ match.capturedLength();
			}
			setFormat(startIndex, commentLength, multiLineCommentFormat);
			startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
		}
	}
	else /*�޸���*/
	{
	}
}