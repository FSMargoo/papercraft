/**
 * \file AssetManager.cpp
 * \brief The asset manager in paper library
 */

#include <include/assets/AssetManager.h>

PAssetManager::PAssetManager() {
	LoadAssets("./assets/textures/blocks", _blockAssets);
	LoadAssets("./assets/textures/items", _itemAssets);

	AddFontResource(_T("./assets/font/minecraft_font.ttf"));
}
const PImage *PAssetManager::GetBlock(const PString &Id) const {
	return _blockAssets.find(Id)->second;
}
const PImage *PAssetManager::GetItem(const PString &Id) const {
	return _itemAssets.find(Id)->second;
}
PString PAssetManager::GetSound(const PString &Id) const {
	return _miscAssets.find(Id)->second;
}
void PAssetManager::LoadMiscAssets(const PString &Path) {
	auto blockPath = std::filesystem::directory_iterator(Path.c_str());
	for (const auto &entry : blockPath) {
		if (entry.is_directory()) {
			auto path = entry.path().string();
			LoadMiscAssets(path.c_str());
		} else {
			auto searchPath = entry.path().string();
			auto id			= entry.path().stem().string();

			_miscAssets.insert(std::make_pair<PString, PString>(PString(id.c_str()), searchPath.c_str()));

			auto instance = PMCIManager::OpenBGM(searchPath.c_str(), searchPath.c_str());
			_miscInstance.insert(std::make_pair<PString, PMCIInstance *>(PString(id.c_str()), std::move(instance)));
		}
	}
}
void PAssetManager::LoadSounds() {
	LoadMiscAssets("./assets/sounds");
}
PMCIInstance *PAssetManager::GetSoundInstance(const PString &Id) const {
	if (_miscInstance.find(Id) != _miscInstance.end()) {
		return _miscInstance.find(Id)->second;
	}

	return nullptr;
}
void PAssetManager::LoadAssets(const PString &RootPath, std::unordered_map<PString, PImage *> &AssetContainer) {
	auto blockPath = std::filesystem::directory_iterator(RootPath.c_str());
	for (const auto &entry : blockPath) {
		auto searchPath = entry.path().string();
		auto id			= entry.path().stem().string();

		AssetContainer.insert(std::make_pair<PString, PImage *>(PString(id.c_str()), new PImage(searchPath.c_str())));
	}
}
PAssetManager::~PAssetManager() {
	RemoveFontResource(_T("./assets/font/minecraft_font.ttf"));
}