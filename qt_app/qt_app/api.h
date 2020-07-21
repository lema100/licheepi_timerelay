#ifndef API_H
#define API_H

#include <QObject>
#include <variant>

#include "endpoints.h"
#include "env.h"
#include "logger.h"

class api : public QObject
{
public:
	api(http::QHttpRequest * req,
		http::QHttpResponse * res,
		env * __env,
		QList<std::shared_ptr<endpoint_base>> list);

	~api();

private:
	env * _env;
	logger * _logger;
	http::QHttpRequest * _req;
	http::QHttpResponse * _res;
	QList<std::shared_ptr<endpoint_base>> _list;

	std::function<void(void)> get_func(void);
};

#endif // API_H
