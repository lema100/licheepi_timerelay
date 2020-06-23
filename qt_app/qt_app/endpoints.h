#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <QStringList>
#include <QDir>

#include "endpoint_base.h"

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
		return "/test";
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
		return "/hello";
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
		return "/selfkill";
	}
};

#endif // ENDPOINTS_H
