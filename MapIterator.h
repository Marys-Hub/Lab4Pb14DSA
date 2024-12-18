#pragma once
#include "Map.h"
class MapIterator
{
    //DO NOT CHANGE THIS PART
    friend class Map;
private:
    Map map;
    int currentIndex;
    //TODO - Representation

    MapIterator(const Map& m);
public:
    void first();
    void next();
    TElem getCurrent();
    bool valid() const;

    void modify(TKey c, TValue v);
};


