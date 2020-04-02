#pragma once
template<typename T> class PingPongValue {
private:
	T min, max, speed, dir{ 1 };
public:
	T value;
public:
	PingPongValue(T mMin, T mMax, T mSpeed, T init);
	void step();
};