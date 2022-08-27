#pragma once

#include <thread>
#include <deque>
#include <string>
#include <sstream>
#include <chrono>
#include <mutex>

#define LOG_TYPE_DEBUG 0x00;
#define LOG_TYPE_INFO 0x01;
#define LOG_TYPE_ERROR 0x02;
#define LOG_TYPE_WARN 0x03;

struct LogData {
	int type = NULL;
	int thread = NULL;
	std::string message;
	std::string Subtype;
	std::chrono::system_clock::time_point time;
	long long int RTime = 0;
};

class LogUtils {
public:

	void Start();
	
	void Stop();

	void LogError(std::string Subtype, std::string Message);

	void LogWarn(std::string Subtype, std::string Message);

	void LogInfo(std::string Subtype, std::string Message);

	void LogDebug(std::string Subtype, std::string Message);

private:

	std::mutex Mutex;

	bool stop = false;

	bool Started = false;

	void MainLogger();

	std::chrono::high_resolution_clock::time_point InitTime = std::chrono::high_resolution_clock::now();

	std::thread LoggingThread;
	
	std::deque<LogData> Logs;
	std::deque<LogData> LogsCache;
};

LogUtils* getLogger();
