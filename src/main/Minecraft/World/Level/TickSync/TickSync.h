#pragma once
#include <chrono>

class TickSync {
public:
	void Start() {

	}

	void SetTPS(unsigned int TPS_) {
		TPS = TPS_;
	}

	void Tick() {
		TickStart = std::chrono::high_resolution_clock::now();
		TicksCount++;
	}

	void ThreadStarting() {
		ThreadsTicking++;
	}

	void ThreadStopping() {
		ThreadsTicking--;
	}

	void CheckIfTickEnds() {

	}

	unsigned int TicksCount = 0;

private:
	unsigned int TPS = 20;
	unsigned int ThreadsTicking = 0;
	std::chrono::steady_clock::time_point TickStart;

};