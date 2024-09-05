/**
* \file BlockMap.h
 * \breif The map structure in the Game
 */

#pragma once

#include "Block.h"

/**
 *  the map of papercraft
 */
class PBlockMap {
public:
	/**
	 * made by block
	 */
	using BlockMap = std::vector<PBlock>;

public:
	PBlockMap() = default;
	~PBlockMap() = default;

public:
	const BlockMap &GetBlockMap() {
		return _block_map;
	}

private:
	BlockMap _block_map;
};