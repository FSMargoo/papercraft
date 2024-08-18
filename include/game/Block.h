///
/// The block structure in the Game
///

#pragma once

#include <include/EasyXBase.h>
#include <include/game/component/Component.h>

/// Yeah, that is actually the "block" hitbox
class PBlockHitboxComponents : public PComponent {
public:
	PBlockHitboxComponents()		   = default;
	~PBlockHitboxComponents() override = default;

public:
};

/// The basic block class in the paper craft
class PBlock {
public:
	/// Register a block, the block of the position of the instance
	/// will be set as zero
	/// \tparam Type The derivative type of the block
	/// \param Id The id of the block
	/// \param Texture The texture of the block, the lifetime of the texture
	/// will be maintained by the unique block instance
	/// \return The block
	template <class Type>
		requires std::is_base_of_v<PBlock, Type>
	std::unique_ptr<Type> RegisterBlock(const PString &Id, PImage *Texture) {
		return new PBlock(Id, 0, 0, Texture);
	}

private:
	/// Construct the block with the texture
	/// \param Id The id of the block
	/// \param X The X axis of the block
	/// \param Y The Y axis of the block
	/// \param Texture The block's texture pointer
	PBlock(const PString &Id, const int &X, const int &Y, PImage *Texture);
	~PBlock() = default;

public:
	/// Whether the block is half brick, if the block is half brick,
	/// the hitbox of the block on vertical will be cut by half
	/// \return If the block is half brick, return false, nor true
	virtual bool IsHalfBrick() {
		return false;
	}
	/// Whether this block is liquid, the block's hitbox will be changed as
	/// liquid hitbox
	/// \return If the block is liquid, return true, nor return false
	virtual bool IsLiquid() {
		return false;
	}
	/// Whether this block is plant, if it is, this block can be destroyed by
	/// water and has only 1 hardness
	/// \return If the block is
	virtual bool IsPlant() {
		return false;
	}

public:
	/// Clone the block with the specified position
	/// \tparam Type The type of the
	/// \return
	template<class Type>
	Type *Clone() {

	}

private:
	int		_x;
	int		_y;
	PString _id;
	PImage *_texture;
};