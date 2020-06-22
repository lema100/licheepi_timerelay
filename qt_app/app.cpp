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
	QObject::connect(&_everySec, &QTimer::timeout, this, &app::on_timeout);

	_everySec.start(1000);
}

void app::on_timeout(void)
{
	qDebug() << QTime::currentTime();
	for (const auto & i : _relay_config)
	{
		qDebug() << i.gpio << i.mode;
		for (const auto & j : i.timeline.keys())
		{
			qDebug() << j << i.timeline[j];
		}
	}
}

app::~app()
{

}
