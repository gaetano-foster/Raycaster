#ifndef TILEMANAGER_H
#define TILEMANAGER_H
#include <olcPixelGameEngine.h>
#include <array>

struct Tile {
    olc::Sprite *tex;
    bool fullbright = true;
    bool solid = true;

    Tile(olc::Sprite *t);
    Tile();
    ~Tile();
};

class TileManager {
public:
    static TileManager& get();
    Tile* get_tile(int index);
    void load();

    TileManager(TileManager const&) = delete;
    void operator=(TileManager const&) = delete;

private:
    Tile *tiles[256];
    TileManager();
    ~TileManager();
};

#endif