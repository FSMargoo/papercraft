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
 * \file ChunkSerialization.h
 * \brief The serialization tool for chunk structure
 */

#pragma once

#include <include/game/chunk/Chunk.h>
#include <include/singleton/Singleton.h>
#include <include/game/manager/BlockIDMapManager.h>

#include <fstream>

constexpr short PBN_MapMagicNumber = 0xfa;
constexpr short PBN_MapVersion	   = 0xf1;
constexpr short PBN_ChunkStart	   = 0x01;
constexpr short PBN_SectionStart   = 0x02;
constexpr short PBN_ChunkEnd	   = 0x03;
constexpr short PBN_SectionEnd	   = 0x04;
constexpr short PBN_Empty		   = 0xb0;

class PChunkSerializationChunkSizeOverflow : public std::exception {
public:
	PChunkSerializationChunkSizeOverflow() {
		_info = "PChunkSerialization failed: Chunk size overflow, it should not greater than 32!";
	}

	const char *what() const throw() override {
		return _info.c_str();
	}

private:
	PString _info;
};

class PChunkSerialization {
public:
	/**
	 * Serialize a chunk and write into std::ofstream
	 * @param Stream The stream to be written
	 * @param ChunkList The chunk list
	 */
	static void Serialization(std::ofstream &Stream, const std::vector<PChunk *> &ChunkList);
};