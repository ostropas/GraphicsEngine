#include "PingPongValue.h"

template<typename T>
PingPongValue<T>::PingPongValue(T mMin, T mMax, T mSpeed, T init) {
	value = init;
	min = mMin;
	max = mMax;
	speed = mSpeed;
}

template<typename T>
void PingPongValue<T>::step()
{
	// allowing over-/underflow
	value += speed * dir;

	// clamp
	value += (min - value) * (value < min) + (max - value) * (value > max);

	// set dir
	dir += 2 * ((value == min) - (value == max));
}
