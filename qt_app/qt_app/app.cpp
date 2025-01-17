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
	_args.addOption(QCommandLineOption("c", QString("Config file (%1.ini by default)").arg(inst_name), "config", inst_name + ".ini"));
	_args.addOption(QCommandLineOption("use-system-proxy", "Use system proxy for network connection"));
	_args.process(*this);
	return true;
}

app::app(int argc, char *argv[]) :
	QCoreApplication(argc, argv),
	_args_init(args()),
	_env(_args.value("c"))
{
	QNetworkProxyFactory::setUseSystemConfiguration(_args.isSet("use-system-proxy"));
	QProcess::execute(QString("ln -sf /usr/share/zoneinfo/uclibc/%1 /etc/TZ").arg(_env.get_global().timezone));

	_listen_state = _httpServer.listen(QString::number(_env.get_global().http_port), [this](http::QHttpRequest * req, http::QHttpResponse * res)
	{
		QList<std::shared_ptr<endpoint_base>> list = {
			std::make_shared<TestEnpoint>(),
			std::make_shared<HelloEndpoint>(),
			std::make_shared<SelfKillEndpoint>(),
			std::make_shared<SettingEnpoint>(),
			std::make_shared<RelayEnpoint>(),
			std::make_shared<EthEnpoint>(),
			std::make_shared<RebootEnpoint>(),
			std::make_shared<StaticEnpoint>(),
		};
		new api(req, res, &_env, list);
	});

	auto str = QString("HTTP listen on %1 port start %2").arg(_env.get_global().http_port).arg(_listen_state ? "OK" : "FALSE");
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
	auto _relay = _env.get_relay();
//	qDebug() << QTime::currentTime();
	for (const auto & i : _relay.keys())
	{
//		qDebug() << relay.gpio << relay.mode;
		switch (_relay[i].mode)
		{
		case TIME:
		{
			auto curr_time = QTime::currentTime();
			auto points = _relay[i].timeline.keys();
			if (points.count() > 1)
			{
				bool state = _relay[i].timeline.last();
				for (const auto & point : points)
				{
					if (curr_time > point)
						state = _relay[i].timeline[point];
					else
						break;
				}
				gpio::set_value(_relay[i].gpio, state);
			}
			else if (points.count() == 1)
				gpio::set_value(_relay[i].gpio, _relay[i].timeline.last());
			else
				gpio::set_value(_relay[i].gpio, 0);
			break;
		}
		case PWM:
		{
			int curr_time = QTime::currentTime().msecsSinceStartOfDay();

			if (_relay[i].pulse_on != 0 && _relay[i].pulse_off != 0)
			{
				int period = _relay[i].pulse_on + _relay[i].pulse_off;
				int pulse_count = curr_time / period;
				curr_time -= pulse_count * period;
				if (curr_time > _relay[i].pulse_on)
					gpio::set_value(_relay[i].gpio, 0);
				else
					gpio::set_value(_relay[i].gpio, 1);
			}
			else
				gpio::set_value(_relay[i].gpio, 0);
			break;
		}
		case MANUAL:
		{
			gpio::set_value(_relay[i].gpio, _relay[i].state);
			break;
		}
		case PULSE:
		case OFF:
		default:
			gpio::set_value(_relay[i].gpio, 0);
		}
	}

	{
		static bool led_state;
		led_state = !led_state;
		gpio::set_value(_env.get_global().gpio_green, led_state);
		gpio::set_value(_env.get_global().gpio_red, 0);
	}
}

app::~app()
{

}
