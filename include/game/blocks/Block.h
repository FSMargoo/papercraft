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
 * The concept of the PBlock type, which has the legit Clone function
 */
template <typename T>
concept PBlockTypeHasClone = requires(T *A, const int &B, const int &C) {
	{ T::Clone(A, B, C) } -> std::same_as<T*>;
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
	 * @param NormalTexture The normal texture of the block
	 * @return The block
	 */
	template <class Type>
		requires std::is_base_of_v<PBlock, Type>
	static Type *RegisterBlock(const PString &Id, PImage *Texture, PImage *NormalTexture) {
		return static_cast<Type*>(new PBlock(Id, 0, 0, Texture, NormalTexture));
	}

public:
	~PBlock() override = default;

protected:
	/**
	 * Construct the block with the texture
	 * @param Id The id of the block
	 * @param X The X axis of the block
	 * @param Y The Y axis of the block
	 * @param Texture The block's texture pointer
	 * @param NormalTexture The normal texture of the block
	 */
	PBlock(const PString &Id, const int &X, const int &Y, PImage *Texture, PImage *NormalTexture);

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
	virtual bool IsLightSource() {
		return false;
	}

public:
	/**
	 * Convert this pointer to the specified type
	 * @tparam Type The specified type to be converted
	 * @return The pointer in specified type
	 */
	template<class Type>
		requires std::is_base_of_v<PBlock, Type>
	Type* Cast() {
		return static_cast<Type*>(this);
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
	/**
	 * Get the normal texture of the block
	 * @return the texture of the block
	 */
	[[nodiscard]] PImage* GetNormalTexture() const {
		return _normalTexture;
	}
	/**
	 * Get the position or hitbox of block
	 * @return the Bound of block
	 */
	[[nodiscard]] const RECT& GetBound() const {
		return Bound;
	}
	/**
	 * Get the light level of the light source
	 * @return The light source level of the light source
	 */
	[[nodiscard]] virtual float GetLightLevel() const {
		return 0.;
	}

public:
	/** Clone the block with the specified position
	 * @tparam Type The type of the block class
	 * @param X The X position of the block
	 * @param Y The Y position of the block
	 * @return The new block pointer
	 */
	template<class Type>
		requires PBlockTypeHasClone<Type> and std::is_base_of_v<PBlock, Type>
	Type *Clone(const int &X, const int &Y) {
		auto object = Type::Clone(static_cast<Type*>(this), X, Y);
		object->_componentList = _componentList;

		return object;
	}

public:
	/**
	 * Clone the PBlock object from a object
	 * @param Block The existed block pointer
	 * @param X The X position of the block
	 * @param Y The Y position of the block
	 * @return The new block in PBlock pointer
	 */
	static PBlock *Clone(PBlock *Block, const int &X, const int &Y) {
		return new PBlock(Block->_id, X, Y, Block->_texture, Block->_normalTexture);
	}

protected:
	int		_x;
	int		_y;
	PString _id;
	PImage *_texture;
	PImage *_normalTexture;
};
/**
 *  the map of papercraft
 */
class PBlockMap {
public:
	/**
	 * made by block
	 */
	using BlockMap = std::vector<PBlock*>;

public:
	PBlockMap(const BlockMap &Map);
	~PBlockMap() = default;

public:
	const BlockMap &GetBlockMap() const {
		return _blockMap;
	}

private:
	BlockMap _blockMap;
};