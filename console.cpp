#include <QtDebug>
#include <QScrollBar>

#include "console.h"

Console::Console(QWidget *parent) :
	QPlainTextEdit(parent),
	qcout(std::cout, QtDebugMsg), /* redirect standard outputs to console */
	qcerr(std::cerr, QtCriticalMsg)
{
	DebugStream::registerConsole(this);
	DebugStream::registerHandler();


}

void Console::setup()
{
	QFile intro(":/resources/intro.html");
	if (!intro.open(QFile::ReadOnly))
		qCritical() << "Failed to open intro.html resource";

	appendHtml(intro.readAll());
	moveCursor(QTextCursor::Start);
}
