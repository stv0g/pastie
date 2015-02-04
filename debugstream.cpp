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

#include "console.h"
#include "debugstream.h"

Console *DebugStream::console;

DebugStream::DebugStream(std::ostream &s, QtMsgType t) :
	stream(s),
	type(t)
{
	oldBuf = s.rdbuf();
	s.rdbuf(this);
}

DebugStream::~DebugStream()
{
	if (!string.empty())
		handler(type, QString::fromStdString(string));

	stream.rdbuf(oldBuf);
}

std::streamsize DebugStream::xsputn(const char *p, std::streamsize n)
{
	string.append(p, p + n);

	size_t pos = 0;
	while (pos != std::string::npos) {
		pos = string.find('\n');
		if (pos != std::string::npos) {
			std::string tmp(string.begin(), string.begin() + pos);
			handler(type, QString::fromStdString(tmp));
			string. erase(string.begin(), string.begin() + pos + 1);
		}
	}

	return n;
}

int DebugStream::overflow(int v)
{
	if (v == '\n') {
		handler(type, QString::fromStdString(string));
		string.clear();
	}
	else
		string += v;

	return v;
}



void DebugStream::registerHandler()
{
	qInstallMessageHandler(DebugStream::handler);
}

void DebugStream::registerConsole(Console *con)
{
	console = con;
}

void DebugStream::handler(QtMsgType type, const QString &msg)
{
	static int dropped;
	static int next = 1;

	// FIXME: dirty workaround
	if (msg == "Camera dropped frame!") {
		if (++dropped == next) {
			next = next + next*2;
			printf("Camera dropped %u frames! Next %u\n\n", dropped, next);
		}

		return;
	}

	if (console)
		console->log(msg, type);

	printf("%s\n", (const char *) QByteArray(msg.toLocal8Bit().constData()));
}

void DebugStream::handler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
	handler(type, msg);
}
