#include "endpoints.h"
#include "app.h"

bool app::args()
{
	QCoreApplication::setApplicationName(DESCRIBE);
	QCoreApplication::setApplicationVersion(VERSION);

	auto inst_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName().split(".").first();

	_args.setApplicationDescription(DESCRIBE);
	_args.addHelpOption();
	_args.addVersionOption();
	_args.addOption(QCommandLineOption("c", QString("Config file (%1.ini by default)").arg(inst_name), "config", inst_name));
	_args.addOption(QCommandLineOption("use-system-proxy", "Use system proxy for network connection"));
	_args.process(*this);
	return true;
}

app::app(int argc, char *argv[]) :
	QCoreApplication(argc, argv),
	_args_init(args()),
	_env(_args.value("c")),
	_relay_config(_env.get_relay())
{
	QNetworkProxyFactory::setUseSystemConfiguration(_args.isSet("use-system-proxy"));

	_listen_state = _httpServer.listen(_env.get_http_port(), [this](http::QHttpRequest * req, http::QHttpResponse * res)
	{
		QList<std::shared_ptr<endpoint_base>> list = {
			std::make_shared<TestEnpoint>(),
			std::make_shared<HelloEndpoint>(),
			std::make_shared<SelfKillEndpoint>(),
		};
		new api(req, res, _env.get_logger(), _env.get_endpoint_conf(), list);
	});

	auto str = QString("HTTP listen on %1 port start %2").arg(_env.get_http_port()).arg(_listen_state ? "OK" : "FALSE");
	qDebug() << str;

	if (_listen_state)
		_env.get_logger()->debug() << str;
	else
		_env.get_logger()->error() << str;

	QObject::connect(&_everySec, &QTimer::timeout, this, &app::on_timeout);
	_everySec.start(1000);
}

void app::on_timeout(void)
{
	qDebug() << QTime::currentTime();
	for (const auto & relay : _relay_config)
	{
		qDebug() << relay.gpio << relay.mode;
		for (const auto & point : relay.timeline.keys())
		{
			qDebug() << point << relay.timeline[point];
		}
	}
}

app::~app()
{

}
