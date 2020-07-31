#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QNetworkProxyFactory>
#include <QFileInfo>
#include <QProcess>
#include <QObject>
#include <QDebug>
#include <QTimer>

#include "gpio.h"
#include "env.h"
#include "api.h"

#define VERSION "v.1.0b"
#define DESCRIBE "Licheepi based timerelay"

class app : public QCoreApplication
{
	Q_OBJECT

public:
	app(int argc, char *argv[]);
	bool get_http_state(void){ return _listen_state; }
	~app();

private:
	QCommandLineParser _args;
	http::QHttpServer _httpServer;
	QTimer _everySec;

	bool args(void);
	bool _args_init;
	bool _listen_state;
	env _env;
	QMap<int, relay_cfg> _relay_config;

public slots:
	void on_timeout(void);
};

#endif // APP_H
