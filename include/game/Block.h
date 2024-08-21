/**
 * \file Block.h
 * \breif The block structure in the Game
 */

#pragma once

#include <include/EasyXBase.h>
#include <include/game/component/Component.h>
#include <include/game/component/Object.h>
#include <vector>

/*
 * Yeah, that is actually the "block" hitbox
 */

class PBlockHitboxComponents : public PComponent {
public:
	PBlockHitboxComponents()		   = default;
	~PBlockHitboxComponents() override = default;

public:
	RECT HitBox() override;

	bool IsOverlap(const RECT& hitbox);

private:
	RECT hitbox;

};

/**
 * The basic block class in the paper craft
 */
class PBlock : public PObject {
public:
	/**
	 * Register a block, the block of the position of the instance
	 * will be set as zero
	 * @tparam Type The derivative type of the block
	 * @param Id The id of the block
	 * @param Texture The texture of the block, the lifetime of the texture
	 * will be maintained by the unique block instance
	 * @return The block
	 */
	template <class Type>
		requires std::is_base_of_v<PBlock, Type>
	Type *RegisterBlock(const PString &Id, PImage *Texture) {
		return new PBlock(Id, 0, 0, Texture);
	}

private:
	/**
	 * Construct the block with the texture
	 * @param Id The id of the block
	 * @param X The X axis of the block
	 * @param Y The Y axis of the block
	 * @param Texture The block's texture pointer
	 */
	PBlock(const PString &Id, const int &X, const int &Y, PImage *Texture);
	~PBlock() = default;

public:
	/** Whether the block is half brick, if the block is half brick,
	 * the hitbox of the block on vertical will be cut by half
	 * @return If the block is half brick, return false, nor true
	 */
	virtual bool IsHalfBrick() {
		return false;
	}
	/**
	 * Whether this block is liquid, the block's hitbox will be changed as
	 * liquid hitbox
	 * @return If the block is liquid, return true, nor return false
	 */
	virtual bool IsLiquid() {
		return false;
	}
	/** Whether this block is plant, if it is, this block can be destroyed by
	 * water and has only 1 hardness
	 * @return If the block is
	 */
	virtual bool IsPlant() {
		return false;
	}

	/**Whether this block is light sources
	 * @return the level of light, 0 means not a light source.
	 */
	virtual int IsLightSource() {
		return 0;
	}

public:
	/**
	 * Get the X position of the block
	 * @return The X position of the block
	 */
	[[nodiscard]] int GetX() const {
		return _x;
	}
	/**
	 * Get the Y position of the block
	 * @return The Y position of the block
	 */
	[[nodiscard]] int GetY() const {
		return _y;
	}
	/**
	 * Get the texture of the block
	 * @return the texture of the block
	 */
	[[nodiscard]] PImage* GetTexture() const {
		return _texture;
	}
	/**Get the position or hitbox of block
	 *@return the Bound of block
	 */
	[[nodiscard]] const RECT& GetBound() {
		return Bound;
	}
	/**
	 *
	 */
	[[nodiscard]] const SkColor& GetColor() {
		return
	}
public:
	/** Clone the block with the specified position
	 * @tparam Type The type of the
	 * @param X
	 * @return
	 */
	template<class Type>
	Type *Clone(const int &X, const int &Y) {

	}

private:
	int		_x;
	int		_y;
	PString _id;
	PImage *_texture;
};

/**
 *  the map of papercraft
 */
class PBlockMap {
	/**
	 * made by block
	 */
	typedef std::vector<std::vector<PBlock>> BlockMap;
public:
	PBlockMap() = default;
	~PBlockMap() = default;

	const BlockMap& GetBlockMap(){return _block_map;}

private:
	BlockMap _block_map;
};