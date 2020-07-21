#include "api.h"

api::api(http::QHttpRequest * req,
		 http::QHttpResponse * res,
		 env * __env,
		 QList<std::shared_ptr<endpoint_base>> list) :
	QObject(req),
	_env(__env),
	_logger(_env->get_logger()),
	_req(req),
	_res(res),
	_list(list)
{
	req->collectData(-1);
	req->onEnd(get_func());
}

std::function<void(void)> api::get_func(void)
{
	return [this]() mutable
	{
		_logger->debug() << QString("From %1:%2 method: %3 url: %4")
							.arg(_req->remoteAddress().toUtf8().constData())
							.arg(_req->remotePort())
							.arg(qhttp::Stringify::toString(_req->method()))
							.arg(_req->url().toString());

		for (auto const & el : _list)
		{
			if (el->path() == _req->url().path())
			{
				if (el->methods().contains(_req->method()))
				{
					endpoint_base::result res;
					el->parse_query(_req);
					el->set_config(_env);

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
