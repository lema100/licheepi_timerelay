#include "logger.h"

#define inline

inline QString logger::levelToString(LogLevel level)
{
	static const char* levels[] = { "Off", "Error", "Warning", "Info", "Debug" };

	return levels[static_cast<int>(level)];
}

inline logger & logger::log(const QString & value, LogLevel level)
{
	if (_log_level >= level)
	{
		QMutexLocker locker(&_mutex);
		QTextStream ts(&_outFile);
		//QTextStream stream(stdout);
		if(level == LogLevel::INFORM)
			ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss,zzz") << " " << value << "\r\n";
		else
			ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss,zzz") << " " << hex << QThread::currentThreadId() << dec << " " << levelToString(level) << " " << value << "\r\n";
		ts.flush();
	}
	return *this;
}

inline logger::logger(const QString & path, const QString & prefix) : _outFile(path), _log_level(LogLevel::ERROR), _outFileName(path), _prefix(prefix)
{
	_outFile.open(QIODevice::WriteOnly | QIODevice::Append);
}

inline void logger::set_log_level(LogLevel level)
{
	QMutexLocker locker(&_mutex);
	_log_level = level;
}

inline log_operator logger::error()
{
	return log_operator(*this, LogLevel::ERROR);
}

inline log_operator logger::warning()
{
	return log_operator(*this, LogLevel::WARNING);
}

inline log_operator logger::info()
{
	return log_operator(*this, LogLevel::INFORM);
}

inline log_operator logger::debug()
{
	return log_operator(*this, LogLevel::DEBUG);
}

inline log_operator::~log_operator()
{
	_logger.log(_res, _level);
}

inline log_operator::log_operator(logger & logger, LogLevel level) : _logger(logger), _level(level)
{
	QTextStream ts(&_res);
	ts << _logger._prefix << ": ";
}

inline log_operator::log_operator(log_operator && lop) : _logger(lop._logger), _level(std::move(lop._level)), _res(std::move(lop._res))
{

}
