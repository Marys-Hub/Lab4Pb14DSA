#include "Map.h"
#include "MapIterator.h"
#include <exception>
using namespace std;


MapIterator::MapIterator(const Map& d) : map(d), currentIndex(-1)
{
    //TODO - Implementation
    first();
}


void MapIterator::first() {
    //TODO - Implementation
    currentIndex = 0;
            //-1; // Set currentIndex to -1 initially

    // Advance currentIndex until a valid position is found or the end of the map is reached
   /*while (currentIndex + 1 < map.m && map.table[currentIndex + 1].first == -1)
    {
        currentIndex++;
    }*/

    while (currentIndex < map.m && map.table[currentIndex].first == -1) {
        currentIndex++;
    }
}


void MapIterator::next() {
    //TODO - Implementation
    if (!this->valid()) {
        throw std::exception();
    }
    this->currentIndex++;
    while (this->currentIndex + 1 < this->map.m && this->map.table[this->currentIndex + 1].first == -1) {
        this->currentIndex++;
    }
}


TElem MapIterator::getCurrent(){
    //TODO - Implementation
    if (!valid()) {
        throw std::exception();
    }
    return map.table[currentIndex];
    return NULL_TELEM;


}


bool MapIterator::valid() const {
    //TODO - Implementation
    return currentIndex >= 0 && currentIndex < map.m && map.table[currentIndex].first != -1;
    return false;
}


void MapIterator::modify(TKey newKey, TValue newValue){
    if (!valid()) {
        throw std::exception();
    }

    // Check if the new key has a different hash value
    int newHash = map.hashFunc(newKey);
    if (newHash != map.hashFunc(map.table[currentIndex].first)) {
        // Remove the current element and add the new key-value pair
        map.remove(map.table[currentIndex].first);
        map.add(newKey, newValue);
    } else {
        // Update the value of the current element
        map.table[currentIndex].second = newValue;
    }
}

//precon: map belongs to Map, pair(k,c) not included in map || tkey belongs keys
//postcond: map inlcluded in map' , mao' = (map U Pair(newStuff))

// complexity: best T(1) -only change current with given value when key exists ,
// worst T(n + m)-when we have to add a new key with value and basically delete the current one
// general O(n + M)
// pseudocode looks kinda the same 

