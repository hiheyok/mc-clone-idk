#pragma once
#include <chrono>

class TickSync {
public:
	void SetTPS(unsigned int TPS_) {
		TPS = TPS_;
	}

	void Tick() {
		TickStartingTime = std::chrono::high_resolution_clock::now();
		TicksCount++;
	}

	void ThreadStarting() {
		ThreadsRunning++;
	}

	void ThreadStopping() {
		ThreadsRunning--;
	}

	void ThreadTicking() {
		ThreadsTicking++;
	}

	void ThreadStopTicking() {
		ThreadsTicking--;
	}

	bool CheckIfTickTimeExceededMinTickTime() {
		double MSPT = (double)(TickStartingTime - std::chrono::high_resolution_clock::now()).count() / 1000000.0;
		return MSPT >= 1000.0 / TPS ? (ThreadsTicking == 0) : false;
	}

	bool CheckIfAllThreadsFinished() {
		return ThreadsTicking == 0;
	}

	unsigned int TicksCount = 0;

private:
	unsigned int TPS = 20;
	unsigned int ThreadsTicking = 0;
	unsigned int ThreadsRunning = 0;
	std::chrono::steady_clock::time_point TickStartingTime;

};