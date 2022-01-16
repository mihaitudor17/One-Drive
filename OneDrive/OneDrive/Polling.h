#pragma once
#include <QTimer>
#include <thread>
#include "ui_Account.h"
#include <filesystem>

class Polling : public QThread
{
	Q_OBJECT

public:
signals:
	void poolingSignal();

private slots:
	void mTimeOut();

public:
	Polling();

protected:
	void run() override;

};