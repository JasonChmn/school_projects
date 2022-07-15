#include "ChronoTime.h"

static std::chrono::high_resolution_clock TimeChrono;
static std::chrono::milliseconds ms;
static std::chrono::duration<float> fsec;

static auto t0 = TimeChrono.now();
static auto t1 = TimeChrono.now();

void ChronoTime::printTimeSince()
{
	t1 = TimeChrono.now();
		fsec = t1 - t0;
		std::cout << fsec.count() << " s";
	t0 = TimeChrono.now();
}

void ChronoTime::resetChrono()
{
	t0 = TimeChrono.now();
}
