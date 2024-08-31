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
 * \file ChunkSerialization.cpp
 * \brief The serialization tool for chunk structure
 */

#include <include/game/chunk/ChunkSerialization.h>

void PChunkSerialization::Serialization(std::ofstream &Stream, const std::vector<PChunk *> &ChunkList) {
	if (ChunkList.size() > 32) {
		throw PChunkSerializationChunkSizeOverflow();
	}

	// Write magic number/version info
	Stream.write(reinterpret_cast<const char *>(&PBN_MapMagicNumber), sizeof(PBN_MapMagicNumber));
	Stream.write(reinterpret_cast<const char *>(&PBN_MapVersion), sizeof(PBN_MapVersion));

	// The size type should be clear!
	auto chunkSize = __int32(ChunkList.size());
	Stream.write(reinterpret_cast<const char *>(&chunkSize), sizeof(__int32));
	if (ChunkList.empty()) {
		Stream.write(reinterpret_cast<const char *>(&PBN_Empty), sizeof(PBN_Empty));
	}

	for (auto &object : ChunkList) {
		Stream.write(reinterpret_cast<const char *>(&PBN_ChunkStart), sizeof(PBN_ChunkStart));
		for (auto &section : object->SectionList) {
			Stream.write(reinterpret_cast<const char *>(&PBN_SectionStart), sizeof(PBN_SectionStart));
			Stream.write(reinterpret_cast<const char *>(&PBN_SectionStart), sizeof(PBN_SectionStart));
			auto sectionX = section->GetX();
			Stream.write(reinterpret_cast<const char *>(&sectionX), sizeof(sectionX));
			if (section->empty()) {
				Stream.write(reinterpret_cast<const char *>(&PBN_Empty), sizeof(PBN_Empty));
			}

			for (auto &block : *section) {
				auto hash = PGetSingleton<PBlockIDMapManager>().GetHash(block->GetID());
				auto Y	  = block->GetY();
				Stream.write(reinterpret_cast<const char *>(&hash), sizeof(unsigned int));
				Stream.write(reinterpret_cast<const char *>(&Y), sizeof(Y));
			}

			Stream.write(reinterpret_cast<const char *>(&PBN_SectionEnd), sizeof(PBN_SectionEnd));
		}

		Stream.write(reinterpret_cast<const char *>(&PBN_ChunkEnd), sizeof(PBN_ChunkEnd));
	}
}