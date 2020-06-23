#ifndef API_H
#define API_H

#include <QObject>
#include <variant>

#include "endpoints.h"
#include "logger.h"

class api : public QObject
{
public:
	api(http::QHttpRequest * req,
		http::QHttpResponse * res,
		logger * logg,
		endpoint_base::config endpoint_conf,
		QList<std::shared_ptr<endpoint_base>> list);

	~api();

private:
	logger * _logger;
	http::QHttpRequest * _req;
	http::QHttpResponse * _res;
	QList<std::shared_ptr<endpoint_base>> _list;

	std::function<void(void)> get_func(endpoint_base::config conf);
};

#endif // API_H
