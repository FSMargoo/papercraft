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
 * \file BlockManager.h
 * \brief The manager of the blocks, it will store a set of the parent instance of blocks
 */

#pragma once

#include <include/String.h>
#include <include/game/blocks/Block.h>

#include <unordered_map>

class PBlockManager {
public:
	PBlockManager() = default;
	~PBlockManager() = default;

public:
	/**
	 * Register a block into the manager
	 * @param ID The ID of the block
	 * @param Block The pointer to the block
	 */
	void Register(const PString &ID, PBlock *Block);
	/**
	 * Get a parent instance of the block in the specified ID
	 * @param ID The ID of the block
	 * @return The block parent pointer
	 */
	PBlock *GetBlockParent(const PString &ID);

private:
	std::unordered_map<PString, PBlock*> _blockMapping;
};