/*
 * This file is part of Pastie
 *
 * Pastie is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Pastie is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pastie. If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @copyright  2015 Steffen Vogel
 * @license	   http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author     Steffen Vogel <steffen.vogel@rwth-aachen.de>
 * @link       http://www.steffenvogel.de
 */

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
