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

void PChunkSerialization::Serialize(std::ofstream &Stream, const std::vector<PChunk *> &ChunkList) {
	if (ChunkList.size() > 32) {
		throw PChunkSerializationChunkSizeOverflow();
	}

	// Write magic number/version info
	Stream.write(reinterpret_cast<const char *>(&PBN_MapMagicNumber), sizeof(PBN_MapMagicNumber));
	Stream.write(reinterpret_cast<const char *>(&PBN_MapVersion), sizeof(PBN_MapVersion));

	auto chunkSize = int(ChunkList.size());
	Stream.write(reinterpret_cast<const char *>(&chunkSize), sizeof(int));
	if (ChunkList.empty()) {
		Stream.write(reinterpret_cast<const char *>(&PBN_Empty), sizeof(PBN_Empty));
	}

	for (auto &object : ChunkList) {
		Stream.write(reinterpret_cast<const char *>(&PBN_ChunkStart), sizeof(PBN_ChunkStart));
		auto chunkX = object->GetX();
		Stream.write(reinterpret_cast<const char *>(&chunkX), sizeof(chunkX));
		if (object->SectionList.empty()) {
			Stream.write(reinterpret_cast<const char *>(&PBN_Empty), sizeof(PBN_Empty));
		}
		else {
			Stream.write(reinterpret_cast<const char *>(&PBN_MapMagicNumber), sizeof(PBN_MapMagicNumber));
		}
		for (auto &section : object->SectionList) {
			auto sectionSize = short(section->Block.size());
			Stream.write(reinterpret_cast<const char *>(&PBN_SectionStart), sizeof(PBN_SectionStart));
			Stream.write(reinterpret_cast<const char *>(&PBN_SectionStart), sizeof(PBN_SectionStart));
			if (section->empty()) {
				Stream.write(reinterpret_cast<const char *>(&PBN_Empty), sizeof(PBN_Empty));
				Stream.write(reinterpret_cast<const char *>(&PBN_SectionEnd), sizeof(PBN_SectionEnd));

				continue;
			} else {
				Stream.write(reinterpret_cast<const char *>(&PBN_MapMagicNumber), sizeof(PBN_MapMagicNumber));
			}
			Stream.write(reinterpret_cast<const char *>(&sectionSize), sizeof(sectionSize));
			auto sectionX = section->GetX();
			Stream.write(reinterpret_cast<const char *>(&sectionX), sizeof(sectionX));

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

	Stream.write(reinterpret_cast<const char *>(&PBN_FileEnd), sizeof(PBN_FileEnd));
}
std::vector<PChunk *> PChunkDeserialization::Deserialize(std::ifstream &Stream) {
	std::vector<PChunk *> chunks;
	short				  magicNumber	= 0;
	short				  versionNumber = 0;

	Stream.read(reinterpret_cast<char *>(&magicNumber), sizeof(magicNumber));
	Stream.read(reinterpret_cast<char *>(&versionNumber), sizeof(versionNumber));

	if (magicNumber != PBN_MapMagicNumber || versionNumber != PBN_MapVersion) {
		throw PChunkDeserializationFailure("Invalid pck file (Version dose not match/Unknown head format)!");
	}

	int chunkSize = 0;
	Stream.read(reinterpret_cast<char *>(&chunkSize), sizeof(chunkSize));

	chunks.reserve(chunkSize * sizeof(PChunk *));

	short opc = 0;
	Stream.read(reinterpret_cast<char *>(&opc), sizeof(opc));
	if (opc == PBN_Empty) {
		return chunks;
	} else if (opc == PBN_ChunkStart) {
		while (!Stream.eof()) {
			int chunkX = 0;
			Stream.read(reinterpret_cast<char *>(&chunkX), sizeof(chunkX));
			Stream.read(reinterpret_cast<char *>(&opc), sizeof(opc));
			if (opc == PBN_Empty) {
				continue;
			}
			std::vector<PSection *> sections;
			while (!Stream.eof()) {
				Stream.read(reinterpret_cast<char *>(&opc), sizeof(opc));
				if (opc != PBN_SectionStart) {
					throw PChunkDeserializationFailure("Invalid pck file (Unknown operation code)!");
				}

				Stream.read(reinterpret_cast<char *>(&opc), sizeof(opc));
				if (opc == PBN_Empty) {
					Stream.read(reinterpret_cast<char *>(&opc), sizeof(opc));

					continue;
				}

				short sectionSize = 0;
				Stream.read(reinterpret_cast<char *>(&sectionSize), sizeof(sectionSize));
				int sectionX = 0;
				Stream.read(reinterpret_cast<char *>(&sectionX), sizeof(sectionX));
				std::vector<PBlock *> blocks;

				for (auto count = 0; count < sectionSize; ++count) {
					unsigned int hashCode = 0;
					int			 blockY	  = 0;
					Stream.read(reinterpret_cast<char *>(&hashCode), sizeof(hashCode));
					Stream.read(reinterpret_cast<char *>(&blockY), sizeof(blockY));

					auto id	   = PGetSingleton<PBlockIDMapManager>().GetID(hashCode);
					auto block = PGetSingleton<PBlockManager>().GetBlockParent(id);
					blocks.emplace_back(PBlock::Clone(block, sectionX, blockY));
				}

				Stream.read(reinterpret_cast<char *>(&opc), sizeof(opc));
				if (opc != PBN_SectionEnd) {
					throw PChunkDeserializationFailure("Invalid pck file (Unknown operation code)!");
				}

				sections.emplace_back(new PSection(blocks, sectionX));
			}
			Stream.read(reinterpret_cast<char *>(&opc), sizeof(opc));
			if (opc == PBN_FileEnd) {
				break;
			}
			else if (opc != PBN_ChunkStart) {
				throw PChunkDeserializationFailure("Invalid pck file (Unknown operation code)!");
			}

			chunks.emplace_back(new PChunk(sections, chunkX));
		}
	} else {
		throw PChunkDeserializationFailure("Invalid pck file (Unknown operation code)!");
	}

	return chunks;
}