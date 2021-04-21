#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <QStringList>
#include <QDir>

#include "endpoint_base.h"
#include "gpio.h"

class StaticEnpoint : public endpoint_base
{
public:
	result get(void) override;
	QList<qhttp::THttpMethod> methods(void) override
	{
		return {qhttp::THttpMethod::EHTTP_GET};
	}
	QString path(void) override
	{
		return "/";
	}
};

class RelayEnpoint : public endpoint_base
{
public:
	result get(void) override;
	result post(void) override;
	QList<qhttp::THttpMethod> methods(void) override
	{
		return {qhttp::THttpMethod::EHTTP_GET, qhttp::THttpMethod::EHTTP_POST};
	}
	QString path(void) override
	{
		return "/api/relay";
	}
};

class SettingEnpoint : public endpoint_base
{
public:
	result get(void) override;
	result post(void) override;
	QList<qhttp::THttpMethod> methods(void) override
	{
		return {qhttp::THttpMethod::EHTTP_GET, qhttp::THttpMethod::EHTTP_POST};
	}
	QString path(void) override
	{
		return "/api/setting";
	}
};

class EthEnpoint : public endpoint_base
{
public:
	result get(void) override;
	result post(void) override;
	QList<qhttp::THttpMethod> methods(void) override
	{
		return {qhttp::THttpMethod::EHTTP_GET, qhttp::THttpMethod::EHTTP_POST};
	}
	QString path(void) override
	{
		return "/api/ethernet";
	}
};

class TestEnpoint : public endpoint_base
{
public:
	result get(void) override;
	QList<qhttp::THttpMethod> methods(void) override
	{
		return {qhttp::THttpMethod::EHTTP_GET};
	}
	QString path(void) override
	{
		return "/api/test";
	}
};

class HelloEndpoint : public endpoint_base
{
public:
	result get(void) override;
	QList<qhttp::THttpMethod> methods(void) override
	{
		return {qhttp::THttpMethod::EHTTP_GET};
	}
	QString path(void) override
	{
		return "/api/hello";
	}
};

class SelfKillEndpoint : public endpoint_base
{
public:
	result get(void) override;
	QList<qhttp::THttpMethod> methods(void) override
	{
		return {qhttp::THttpMethod::EHTTP_GET};
	}
	QString path(void) override
	{
		return "/api/selfkill";
	}
};

#endif // ENDPOINTS_H
