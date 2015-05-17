/*! \file highlighter.h
	\author HiddenSeeker

	Describes a highlighter, based on QScriptSyntaxHighlighter, but with
	custom functions and words highlighting
 */
#include <QtCore/qglobal.h>

#include <QtGui/qsyntaxhighlighter.h>

#include <QtGui/qtextformat.h>

namespace gui
{

namespace codeedit
{
/*! A highlighter for code in editor
 */
class Highlighter : public QSyntaxHighlighter
{
public:
	/*! Constructs new highlighter 
	 */
    Highlighter(QTextDocument *document = 0);
	/*! Could be inherited
	 */
	virtual ~Highlighter();
	/*! Sets predefined constants
		\param[in] list a list
	 */
	static void setPredefinedConstants(
		const QStringList & list
	);
	/*! Sets predefined functions
		\param[in] list a list
	 */
	static void setPredefinedFunctions(
		const QStringList & list
	);
protected:
	/*! Highlights block
		\param[in] text
	 */
    void highlightBlock(const QString &text);
private:
	/*! Highlights specific word
		\param[in] currentPos current position
		\param[in] buffer a buffer
	 */
    void highlightWord(int currentPos, const QString &buffer);

    enum ScriptFormats {
        ScriptTextFormat, ScriptNumberFormat,
        ScriptStringFormat, ScriptTypeFormat,
        ScriptKeywordFormat, ScriptPreprocessorFormat,
        ScriptLabelFormat, ScriptCommentFormat,
		ScriptPredefinedConstant,
		ScriptPredefinedFunction,
        NumScriptFormats, 
    };
    QTextCharFormat m_formats[NumScriptFormats];

private:
    Q_DISABLE_COPY(Highlighter)
};

}

}
