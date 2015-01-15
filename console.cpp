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

	setCenterOnScroll(true);
	setMaximumBlockCount(1000);
}

void Console::setup()
{
	QFile intro(":/resources/intro.html");
	if (!intro.open(QFile::ReadOnly))
		qCritical() << "Failed to open intro.html resource";

	appendHtml(intro.readAll());
	moveCursor(QTextCursor::Start);
}

void Console::log(QString msg, QtMsgType type)
{
	appendHtml(format(msg, type));
	ensureCursorVisible();
	moveCursor(QTextCursor::End);
	moveCursor(QTextCursor::StartOfLine);
}

QString Console::format(QString msg, QtMsgType type)
{
	QString out("<pre><div style='font-weight: bold; color: %1'>[%2]</div> %3</pre>");

	switch (type) {
		case QtDebugMsg:    out = out.arg("Gray").arg("Debug   "); break;
		case QtWarningMsg:  out = out.arg("Orange").arg("Warning "); break;
		case QtCriticalMsg: out = out.arg("DarkRed").arg("Critical"); break;
		case QtFatalMsg:    out = out.arg("Magenta").arg("Fatal   "); break;
	}

	return out.arg(msg);
}
