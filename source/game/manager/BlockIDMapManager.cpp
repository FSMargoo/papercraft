/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file BlockIDMapManager.cpp
 * \brief The id mapping manager of the block
 */

#include <include/game/manager/BlockIDMapManager.h>

PBlockIDMapManager::PBlockIDMapManager() {
	auto blockPath = std::filesystem::directory_iterator("./assets/textures/blocks");
	for (const auto &entry : blockPath) {
		auto id	  = entry.path().stem().string();
		auto hash = HashValue(id.c_str());
		_mapping.insert({hash, id.c_str()});
		_reserveMapping.insert({id.c_str(), hash});
	}
}
unsigned int PBlockIDMapManager::GetHash(const PString &ID) {
	return _reserveMapping.find(ID)->second;
}
PString PBlockIDMapManager::GetID(const unsigned int &Hash) {
	return _mapping.find(Hash)->second;
}
unsigned int PBlockIDMapManager::HashValue(const PString &Input) {
	unsigned int		   hash	 = 0;
	constexpr unsigned int prime = 31;
	constexpr unsigned int mod	 = INT_MAX;

	for (size_t i = 0; i < Input.size(); ++i) {
		hash = (hash * prime + static_cast<char32_t>(Input[i])) % mod;
	}

	return hash;
}