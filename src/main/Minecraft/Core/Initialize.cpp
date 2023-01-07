#include "Initialize.h"
#include "Registry.h"
#include "../../Utils/LogUtils.h"
void Initialize() {
	for (const auto& obj : BlockRegistry.Container) {
		obj.second->Build();
		getLogger()->LogInfo("Initialize", "(Block) Built: " + BlockRegistry.getName(obj.first));
	}
	for (const auto& obj : EntityRegistry.Container) {
		obj.second->Build();
		getLogger()->LogInfo("Initialize", "(Entity) Built: " + EntityRegistry.getName(obj.first));
	}
	for (const auto& obj : DimensionRegistry.Container) {
		obj.second->Build();
		getLogger()->LogInfo("Initialize", "(Dimension) Built: " + DimensionRegistry.getName(obj.first));
	}
	for (const auto& obj : LevelRegistry.Container) {
		obj.second->Build();
		getLogger()->LogInfo("Initialize", "(Level) Built: " + LevelRegistry.getName(obj.first));
	}
}