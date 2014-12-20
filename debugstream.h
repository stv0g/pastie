#ifndef DEBUGSTREAM_H
#define DEBUGSTREAM_H

#include <QtGlobal>
#include <QPlainTextEdit>

#include <iostream>

class DebugStream : public std::basic_streambuf<char>
{

public:
	DebugStream(std::ostream &stream, QtMsgType type);
	~DebugStream();

	static QString format(QString msg, QtMsgType type = QtDebugMsg);
	static void registerConsole(QPlainTextEdit *);
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
	static QPlainTextEdit *console;
};

#endif // DEBUGSTREAM_H
