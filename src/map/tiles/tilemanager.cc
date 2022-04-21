#include "tilemanager.hh"

Tile::Tile(olc::Sprite *t) {
    this->tex = t;
}

Tile::Tile() {
    
}

Tile::~Tile() {
    delete tex;
}

//////////////////////////////////////////////////////////////////////////

TileManager& TileManager::get() {
    static TileManager me;
    return me;
}

Tile* TileManager::get_tile(int index) {
    return tiles[index];
}

void TileManager::load() {
    tiles[1] = new Tile(new olc::Sprite("res/techwall.png"));
    tiles[2] = new Tile(new olc::Sprite("res/hexwall.png"));
}

TileManager::TileManager() {

}

TileManager::~TileManager() {
    delete tiles[1];
    delete tiles[2];
}