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

#ifndef DEBUGSTREAM_H
#define DEBUGSTREAM_H

#include <QtGlobal>
#include <QPlainTextEdit>

#include <iostream>

class Console;

class DebugStream : public std::basic_streambuf<char>
{

public:
	DebugStream(std::ostream &stream, QtMsgType type);
	~DebugStream();

	static QString format(QString msg, QtMsgType type = QtDebugMsg);
	static void registerConsole(Console *);
	static void registerHandler();

protected:
	int overflow(int_type v);
	std::streamsize xsputn(const char *p, std::streamsize n);

private:
	std::ostream &stream;
	std::streambuf *oldBuf;
	std::string string;

	QtMsgType type;

	static void handler(QtMsgType type, const QString &msg);
	static void handler(QtMsgType type, const QMessageLogContext &, const QString &str);
	static Console *console;
};

#endif // DEBUGSTREAM_H
