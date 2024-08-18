///
/// The block structure in the Game
///

#pragma once

#include "../EasyXBase.h"

/// Yeah, that is actually the "block" hitbox
class PBlockHitboxComponents
{
/// TODO
};

/// The basic block class in the paper craft
class PBlock
{
public:
    /// Construct the block with the texture
    /// @param Texture The block's texture pointer
    PBlock(PImage *Texture);
    ~PBlock() = default;

public:
    /// Whether the block is half brick, if the block is half brick,
    /// the hitbox of the block on vertical will be cut by half
    /// @return If the block is half brick, return false, nor true
    virtual bool IsHalfBrick()
    {
        return false;
    }
    /// Whether this block is liquid, the block's hitbox will be changed as
    /// liquid hitbox
    /// @return If the block is liquid, return true, nor return false
    virtual bool IsLiquid()
    {
        return false;
    }
    /// Whether this block is plant, if it is, this block can be destroyed by
    /// water and has only 1 hardness
    /// @return If the block is 
    virtual bool IsPlant()
    {
        return false;
    }

    /// TODO

private:
    PImage *_texture;
};