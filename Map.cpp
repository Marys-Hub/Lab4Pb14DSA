#include "Map.h"
#include "MapIterator.h"
// 14.ADT Map – repräsentiert mithilfe einer Hashtabelle mit coalesced Verkettung.

// Theta(capacity m)
Map::Map() {
    //TODO - Implementation
    this->m = 13;  // Set the initial value of m

    // Initialize the table and next arrays
    this->table = new TElem[this->m];
    this->next = new int[this->m];
    for (int i = 0; i < this->m; i++) {
        this->table[i].first = -1;  // Initialize the keys to -1
        this->next[i] = -1;  // Initialize the next pointers to -1
    }

    this->firstEmpty = 0;  // Set the firstFree position to 0

}
Map::~Map() {
    //TODO - Implementation
    // Clean up the allocated memory
    delete[] this->table;
    delete[] this->next;
}

// O(m)
TValue Map::add(TKey c, TValue v) {
    if (size() == m) {
        resize(); // Resize varianta 2
    }

    if (firstEmpty == -1) {
        resize(); // Resize the map if there are no more empty slots
    }

    int i = hashFunc(c); // Compute the hash value for the given key
    int j = -1; // Variable to keep track of the previous index in the chain
    while (i != -1 && table[i].first != c) { // Search for the key in the chain
        j = i;
        i = next[i];
    }
    if (i != -1) { // Key already exists in the map
        TValue oldValue = table[i].second;
        table[i].second = v; // Update the value associated with the key
        return oldValue; // Return the old value associated with the key
    } else {
        if (firstEmpty == -1) {
            resize(); // Resize the map if there are no more empty slots after resizing once
            i = hashFunc(c);
            j= -1;
            while (i != -1 && table[i].first != c) { // Search for the key in the chain
                j = i; // Keep track of the previous index in the chain
                i = next[i]; // Move to the next index in the chain
            }
        }
        if (i == -1) { // Key doesn't exist, add it as a new pair
            i = firstEmpty; // Get the next available empty slot
            firstEmpty = next[firstEmpty]; // Update the firstEmpty pointer
            table[i].first = c;
            table[i].second = v;
            next[i] = -1; // Mark the end of the chain
            if (j != -1) {
                next[j] = i; // Update the next pointer of the previous index
            }

            return NULL_TVALUE; // Element was added as a new pair
        } else {
            return table[i].second; // Existing pair found, return the old value
        }
    }
}


// 𝑂(m) im durschnittlicher Fall, aber T(1) best
TValue Map::search(TKey k) const{
    //TODO - Implementation
    int i = hashFunc(k);  // Get the hash position for the key

    while (i != -1 && table[i].first != k) {
        i = next[i];  // Traverse to the next position
    }

    if (i == -1) {
        return NULL_TVALUE;  // Key not found
    } else {
        return table[i].second;  //return i;// Return the associated value;  // Return the position of the found key
    }

    return NULL_TVALUE;
}

// O(n)
TValue Map::remove(TKey k) {
    int i = hashFunc(k); // Compute the hash value for the given key
    int j = -1; // Variable to keep track of the previous index in the chain
    int l = 0;

    // Find the index j that points to index i in the 'next' array
    while (l < m && j == -1) {
        if (next[l] == i) {
            j = l;
        }
        l++;
    }

    // Search for the key in the chain
    while (i != -1 && table[i].first != k) {
        j = i;
        i = next[i];
    }

    if (i == -1) {
        return 0; // Key not found
    }

    bool over = false; // Flag indicating if nothing is hashed to index i

    // Find the next index in the collision chain that is hashed to index i
    do {
        int p = next[i];
        int pp = i; // Variable to keep track of the previous index of p ?????
        while (p != -1 && hashFunc(table[p].first) != i) {
            pp = p;
            p = next[p];
        }
        if (p == -1) {
            over = true; // No more elements hashed to index i
        }
        else {
            i = p; // Move to the next index in the collision chain
        }
    } while (!over);

    // Update the 'next' array to remove the element at index i
    if (j != -1) {
        next[j] = next[i];
    }
    else {
        next[hashFunc(k)] = next[i];
    }

    table[i].first = 0; // Reset the key at index i
    next[i] = firstEmpty; // Mark index i as empty
    firstEmpty = i; // Update the firstEmpty pointer

    return NULL_TVALUE; // Element was removed successfully, return a default value indicating success
}


//Theta(n) general ca gen m e cap
int Map::size() const {
    //TODO - Implementation
    int count = 0;

    for (int i = 0; i < m; i++) {
        if (table[i].first != -1) { // Slot is not empty
            int index = i;
            while (index != -1) {
                count++;
                index = next[index];
            }
        }
    }

    return count;

    return 0;
}
// Om)
bool Map::isEmpty() const{
    //TODO - Implementation
    for (int i = 0; i < m; i++) {
        if (table[i].first != -1) {
            return false;
        }
    }
    return true; // No elements found, map is empty

    return false;
}

MapIterator Map::iterator() const {
    return MapIterator(*this);
}

int Map::hashFunc(TKey k) const {
    return k % m;
}

void Map::resize() {
    int newM = getNextPrime(2 * m);  // Calculate the new size as the next prime number greater than double the current size
    rehash(newM);  // Rehash the elements to the new hash table size
}

int Map::getNextPrime(int n) {
    if (n <= 1) {
        return 2;
    }
    int prime = n + 1;
    while (!isPrime(prime)) {
        prime++;
    }
    return prime;
    return 0;
}

bool Map:: isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

void Map::rehash(int newM) {
    TElem* oldTable = table;
    int* oldNext = next;
    table = new TElem[newM];
    next = new int[newM];

    // Initialize the new hash table and next array
    for (int i = 0; i < newM; i++) {
        table[i].first = 0;
        next[i] = -1;
    }

    int oldM = m;
    m = newM;
    firstEmpty = 0;

    // Rehash the elements from the old hash table to the new hash table
    for (int i = 0; i < oldM; i++) {
        if (oldTable[i].first != 0) {
            int j = hashFunc(oldTable[i].first);

            if (table[j].first != 0) {
                int p = firstEmpty;
                while (table[p].first != 0) {
                    p = (p + 1) % m;
                }
                firstEmpty = p + 1;

                table[p] = oldTable[i];

                if (p < j) {
                    int aux = p;
                    p = j;
                    j = aux;
                }

                for (int q = j; q < p; q++) {
                    if (table[q].first == 0) {
                        table[q].first = -1;
                    }
                }
            } else {
                table[j] = oldTable[i];
            }
        }
    }

    delete[] oldTable;  // Delete the old hash table
    delete[] oldNext;  // Delete the old next array
}






