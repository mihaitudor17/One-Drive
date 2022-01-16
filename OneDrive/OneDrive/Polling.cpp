#include "Polling.h"

void Polling::mTimeOut()
{
	emit(poolingSignal());
	QTimer::singleShot(5000, this, SLOT(mTimeOut()));
}

Polling::Polling()
{
}

void Polling::run()
{
	{
		QTimer::singleShot(0, this, SLOT(mTimeOut()));
		exec();
	}
}