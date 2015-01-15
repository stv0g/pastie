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
