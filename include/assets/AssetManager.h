/**
 * \file AssetManager.h
 * \brief The asset manager in paper library
 */

#pragma once

#include <filesystem>
#include <unordered_map>

#include "../mic/MCIManager.h"

#include "../EasyXBase.h"

/**
 * The asset manager in paper library, it will load the assets file
 * of item and block and provide a API for fetching; In addition, it
 * will also install the Minecraft font throw Win32 API
 */
class PAssetManager {
public:
	/**
	 * Create the asset manager from the local asset folder
	 */
	PAssetManager();
	~PAssetManager();

public:
	/**
	 * Get the block texture by the block id
	 * @param Id The id of the block
	 * @return The const pointer referred to the texture of the block
	 */
	const PImage *GetBlock(const PString &Id) const;
	/*
	 * Get the item texture by the item id
	 * @param Id The id of the item
	 * @return The const pointer referred to the item of the block
	 */
	const PImage *GetItem(const PString &Id) const;
	/**
	 * Get the sound file path by the id of the sound
	 * @param Id The id of the sound
	 * @return The path of the sound file
	 */
	PString GetSound(const PString &Id) const;
	/**
	 * Get the sound instance by the id of the sound
	 * @param Id The id of the sound
	 * @return The instance of the sound
	 */
	PMCIInstance *GetSoundInstance(const PString &Id) const;

public:
	/**
	 * Load the sounds MCI files
	 */
	void LoadSounds();

private:
	/**
	 * Load asset to the mapping cotainer by the root path of the asset
	 * @param RootPath The root path of the resource file
	 * @param AssetContainer The map container for asset files
	 */
	static void LoadAssets(const PString &RootPath, std::unordered_map<PString, PImage *> &AssetContainer);
	/**
	 * Load the music assets and store in a mapping form like id->path
	 * @param Path The path to music file folder
	 */
	void LoadMiscAssets(const PString &Path);

private:
	std::unordered_map<PString, PImage *>		_blockAssets;
	std::unordered_map<PString, PImage *>		_itemAssets;
	std::unordered_map<PString, PString>		_miscAssets;
	std::unordered_map<PString, PMCIInstance *> _miscInstance;
};