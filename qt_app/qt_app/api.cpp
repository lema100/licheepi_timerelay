#include "api.h"

api::api(http::QHttpRequest * req,
		 http::QHttpResponse * res,
		 logger * logg,
		 endpoint_base::config endpoint_conf,
		 QList<std::shared_ptr<endpoint_base>> list) :
	QObject(req),
	_logger(logg),
	_req(req),
	_res(res),
	_list(list)
{
	req->collectData(-1);
	req->onEnd(get_func(endpoint_conf));
}

std::function<void(void)> api::get_func(endpoint_base::config conf)
{
	return [this, conf]()
	{
		_logger->debug() << QString("From %1:%2 method: %3 url: %4")
						  .arg(_req->remoteAddress().toUtf8().constData())
						  .arg(_req->remotePort())
						  .arg(qhttp::Stringify::toString(_req->method()))
						  .arg(_req->url().path());
		for (auto const & el : _list)
		{
			if (el->path().startsWith(_req->url().path()))
			{
				if (el->methods().contains(_req->method()))
				{
					endpoint_base::result res;
					el->parse_query(_req);
					el->set_config(conf);

					switch(_req->method())
					{
					case qhttp::THttpMethod::EHTTP_GET:
						res = el->get();
						break;
					case qhttp::THttpMethod::EHTTP_POST:
						res = el->post();
						break;
					default:
						endpoint_base::res_error(_res, "method not supported by engine");
						return;
					}

					if(!res._headers.contains("content-type"))
						_res->addHeaderValue("content-type", QString("application/json"));
					for(auto key : res._headers.keys())
						_res->addHeaderValue(key.toLatin1(), res._headers[key]);
					_res->setStatusCode(res._status);
					_res->end(res._body);

					return;
				}
				else
				{
					endpoint_base::res_error(_res, "method not exist in endpoint support list");
					return;
				}
			}
		}
		endpoint_base::res_error(_res, "path not exist");
	};
}

api::~api()
{

}
