#pragma once
#include <chrono>
unsigned int SyncTPS = 20;
unsigned int SyncThreadsTicking = 0;
unsigned int SyncThreadsRunning = 0;
unsigned int SyncTicksCount = 0;

std::chrono::steady_clock::time_point SyncTickStartingTime;

void SyncSetTPS(unsigned int TPS_) {
	SyncTPS = TPS_;
}

void SyncTick() {
	SyncTickStartingTime = std::chrono::high_resolution_clock::now();
	SyncTicksCount++;
}

void SyncThreadStarting() {
	SyncThreadsRunning++;
}

void SyncThreadStopping() {
	SyncThreadsRunning--;
}

void SyncThreadTicking() {
	SyncThreadsTicking++;
}

void SyncThreadStopTicking() {
	SyncThreadsTicking--;
}

bool SyncCheckIfTickTimeExceededMinTickTime() {
	double MSPT = (double)(SyncTickStartingTime - std::chrono::high_resolution_clock::now()).count() / 1000000.0;
	return MSPT >= 1000.0 / SyncTPS ? (SyncThreadsTicking == 0) : false;
}

bool SyncCheckIfAllThreadsFinished() {
	return SyncThreadsTicking == 0;
}


