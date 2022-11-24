#include "DimensionNode.h"
#include "../../../../Utils/Clock.h"

void DimensionNode::Run() {
	Sync.ThreadStarting();
	getLogger()->LogDebug("Dimension Node", "Starting Thread ID: " + std::to_string(NodeID));
	while (true) {
		Sync.ThreadTicking();
		Tick();
		Sync.ThreadStopTicking();
		//Sync
		
		if (Sync.TicksCount == TicksCount) {
			if (Sync.CheckIfAllThreadsFinished() && Sync.CheckIfTickTimeExceededMinTickTime()) {
				Sync.Tick();
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