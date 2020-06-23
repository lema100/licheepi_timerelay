#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QTime>
#include <QMessageLogger>
#include <functional>
#include <memory>
#include <QMutex>
#include <QThread>
#include <utility>

enum class LogLevel
{
	OFF,
	ERROR,
	WARNING,
	INFORM,
	DEBUG,
};

class logger;

class log_operator
{
public:
	log_operator(logger & logger, LogLevel level);
	log_operator(log_operator && lop);

	~log_operator();
	
	template <typename T>
	log_operator & operator << (const T & value)
	{
		QTextStream ts(& _res);
		ts << value;
		return *this;
	}
private:
	log_operator & operator = (log_operator && lop) = delete;
	log_operator(const log_operator &) = delete;
	log_operator & operator = (const log_operator &) = delete;

	logger & _logger;
	LogLevel _level;
	QString _res;
};

class logger 
{
public:
	logger(const QString &path, const QString & prefix);

	logger(const logger &) = delete;
	logger & operator = (const logger & ) = delete;

	void set_log_level(LogLevel level);
	log_operator error();
	log_operator warning();
	log_operator info();
	log_operator debug();
	bool isNull() {return null;}
private:
	QFile _outFile;
	LogLevel _log_level;
	QString _outFileName;
	const QString _prefix;

	QMutex _mutex;
	static QString levelToString(LogLevel level);
	logger & log(const QString & value, LogLevel level);

	bool null;
	friend class log_operator;
};

#endif // LOGGER_H
