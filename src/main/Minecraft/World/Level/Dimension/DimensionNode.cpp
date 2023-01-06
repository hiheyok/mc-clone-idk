#include "DimensionNode.h"
#include "../../../../Utils/Clock.h"

void DimensionNode::Run() {
	SyncThreadStarting();
	getLogger()->LogDebug("Dimension Node", "Starting Thread ID: " + std::to_string(NodeID));
	while (true) {
		SyncThreadTicking();
		Tick();
		SyncThreadStopTicking();
		//Sync
		
		if (SyncTicksCount == TicksCount) {
			if (SyncCheckIfAllThreadsFinished() && SyncCheckIfTickTimeExceededMinTickTime()) {
				SyncTick();
			}
			else {
				timerSleep(0.005);
			}
		}


	}
}

void DimensionNode::Stop () {
	if (!stop) {
		stop = true;
		NodeThread.join();
		DProperties = nullptr;
	}
	getLogger()->LogDebug("Dimension Node", "Stopped Thread ID: " + std::to_string(NodeID));
}

void DimensionNode::Tick() {

}