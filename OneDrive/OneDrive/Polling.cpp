#include "Polling.h"


void Polling::mTimeOut()
{
	//lock emit??
	emit(poolingSignal());
	//unlock emit??
	QTimer::singleShot(1000, this, SLOT(mTimeOut()));  //i need some sort of Qmutex
}
