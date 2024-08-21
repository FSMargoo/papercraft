class PBlockInterface;
class PChunkInterface {
public:
    std::vector<PBlockInterface *> BlockInChunk;
};
class PBlockInterface {
public:
    virtual int GetX() const;
    virtual int GetY() const;
    virtual PImage GetTexture() const;
    virtual bool IsLightSource() const;
};