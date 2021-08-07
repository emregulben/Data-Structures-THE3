#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"
#include "ItemNotFoundException.h"
/* Do not add new libraries or files */

#define BUCKET_SIZE 2

// Do not modify the public interface of this class.
// Otherwise, your code will note compile!
template <class K, class T>
class HashTable {
    struct Entry {
        K Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[BUCKET_SIZE];
    };

    int _capacity; // INDICATES THE TOTAL CAPACITY OF THE TABLE
    int _size; // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket* _table; // THE HASH TABLE

    // == DEFINE HELPER METHODS & VARIABLES BELOW ==
    int contains(const K& key) const;

public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    // COPY THE WHOLE CONTENT OF RHS INCLUDING THE KEYS THAT WERE SET AS DELETED
    HashTable(const HashTable<K, T>& rhs);
    HashTable<K, T>& operator=(const HashTable<K, T>& rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE. IF THE LOAD FACTOR OF THE TABLE IS GREATER THAN 0.6,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(const K& key, const T& value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, THROW ItemNotFoundException()
    void Delete(const K& key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THROW ItemNotFoundException()
    T& Get(const K& key) const;

    // TODO: IMPLEMENT THIS FUNCTION.
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES
    void Resize(int newCapacity);

    // THE IMPLEMENTATION OF THESE FUNCTIONS ARE GIVEN TO YOU
    // DO NOT MODIFY!
    int Capacity() const; // RETURN THE TOTAL CAPACITY OF THE TABLE
    int Size() const; // RETURN THE NUMBER OF ACTIVE ITEMS
    void getKeys(K* keys); // PUT THE ACTIVE KEYS TO THE GIVEN INPUT PARAMETER
};


template <class K, class T>
HashTable<K, T>::HashTable() {
    // TODO: IMPLEMENT THIS FUNCTION.
    _capacity = NextCapacity(0);
    _size = 0;
    _table = new Bucket[_capacity];
}

template <class K, class T>
HashTable<K, T>::HashTable(const HashTable<K, T>& rhs) {
    // TODO: IMPLEMENT THIS FUNCTION.
    _capacity = rhs.Capacity();
    _size = rhs.Size();
    _table = new Bucket[_capacity];
    
    for (int i=0; i< _capacity; i++) {
        Bucket& bucket1 = _table[i];
        Bucket& bucket2 = rhs._table[i];
        for (int j=0; j<BUCKET_SIZE; j++) {
            bucket1.entries[j].Key = bucket2.entries[j].Key;
            bucket1.entries[j].Value = bucket2.entries[j].Value;
            bucket1.entries[j].Deleted = bucket2.entries[j].Deleted;
            bucket1.entries[j].Active = bucket2.entries[j].Active;
        }
    }
}

template <class K, class T>
HashTable<K, T>& HashTable<K, T>::operator=(const HashTable<K, T>& rhs) {
    // TODO: IMPLEMENT THIS FUNCTION.
    if (this != &rhs) {
        _capacity = rhs.Capacity();
        _size = rhs.Size();
        delete[] _table;
        _table = new Bucket[_capacity];
        
        for (int i=0; i< _capacity; i++) {
            Bucket& bucket1 = _table[i];
            Bucket& bucket2 = rhs._table[i];
            for (int j=0; j<BUCKET_SIZE; j++) {
                bucket1.entries[j].Key = bucket2.entries[j].Key;
                bucket1.entries[j].Value = bucket2.entries[j].Value;
                bucket1.entries[j].Deleted = bucket2.entries[j].Deleted;
                bucket1.entries[j].Active = bucket2.entries[j].Active;
            }
        }
    }
    return *this;
}

template <class K, class T>
HashTable<K, T>::~HashTable() {
    // TODO: IMPLEMENT THIS FUNCTION.
    delete[] _table;
    
}

template <class K, class T>
int HashTable<K, T>::contains(const K& key) const {
    int index = Hash(key) % _capacity;
    int index_first = Hash(key) % _capacity;
    int k = 1;
    
    while (k != _capacity) {
        Bucket& bucket = _table[index];
        for (int i=0; i<BUCKET_SIZE; i++) {
            if (bucket.entries[i].Key == key && (bucket.entries[i].Deleted == true || bucket.entries[i].Active == true)) {
                return index;
            }
            else if (bucket.entries[i].Deleted == false && bucket.entries[i].Active == false) {
                return -1;
            }
        }
        index = (index_first + k*k) % _capacity;
        k++;
    }
    return -1;
}

template <class K, class T>
void HashTable<K, T>::Insert(const K& key, const T& value) {
    // TODO: IMPLEMENT THIS FUNCTION.
    int cont = contains(key);
    
    if (cont != -1) {
        Bucket& bucket = _table[cont];
        for (int i=0; i<BUCKET_SIZE; i++) {
            if (bucket.entries[i].Key == key && bucket.entries[i].Active == true) {
                bucket.entries[i].Key = key;
                bucket.entries[i].Value = value;
                bucket.entries[i].Deleted = false;
                bucket.entries[i].Active = true;
                return;
            }
        }
    }
    
    int index = Hash(key) % _capacity;
    int index_first = Hash(key) % _capacity;
    int k = 1;
    bool check = true;
    while (check) {
        Bucket& bucket = _table[index];
        for (int i=0; i<BUCKET_SIZE; i++) {
            if (!(bucket.entries[i].Active)) {
                bucket.entries[i].Key = key;
                bucket.entries[i].Value = value;
                bucket.entries[i].Deleted = false;
                bucket.entries[i].Active = true;
                _size++;
                check = false;
                break;
            }
        }
        
        index = (index_first + k*k) % _capacity;
        k++;
    }
    double factor = (double)_size / (BUCKET_SIZE*_capacity);
    if (factor > 0.6) {
        Resize(NextCapacity(_capacity));
    }
}

template <class K, class T>
void HashTable<K, T>::Delete(const K& key) {
    // TODO: IMPLEMENT THIS FUNCTION.
    int cont = contains(key);
    if (cont == -1) {
        throw ItemNotFoundException();
    }
    else {
        Bucket& bucket = _table[cont];
        for (int i=0; i<BUCKET_SIZE; i++) {
            if (bucket.entries[i].Key == key && bucket.entries[i].Active == true) {
                bucket.entries[i].Active = false;
                bucket.entries[i].Deleted = true;
                _size--;
                return;
            }
        }
    }
}

template <class K, class T>
T& HashTable<K, T>::Get(const K& key) const {
    // TODO: IMPLEMENT THIS FUNCTION.
    int cont = contains(key);
    if (cont == -1) {
        throw ItemNotFoundException();
    }
    else {
        Bucket& bucket = _table[cont];
        for (int i=0; i<BUCKET_SIZE; i++) {
            if (bucket.entries[i].Key == key && bucket.entries[i].Active == true) {
                return bucket.entries[i].Value;
            }
        }
        throw ItemNotFoundException();
    }
}


template <class K, class T>
void HashTable<K, T>::Resize(int newCapacity) {
    // TODO: IMPLEMENT THIS FUNCTION.
    int old_capacity = _capacity;
    Bucket* old_table = new Bucket[newCapacity];
    Bucket* temp = _table;
    _table = old_table;
    old_table = temp;
    
    _size = 0;
    _capacity = newCapacity;
    for (int i=0; i < old_capacity; i++) {
        Bucket& bucket = old_table[i];
        for (int j=0; j<BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active == true) {
                Insert(bucket.entries[j].Key, bucket.entries[j].Value);
            }
        }
    }
    delete[] old_table;
}


template <class K, class T>
int HashTable<K, T>::Capacity() const {
    return _capacity;
}

template <class K, class T>
int HashTable<K, T>::Size() const {
    return _size;
}


template <class K, class T>
void HashTable<K, T>::getKeys(K* keys) {
    int index = 0;

    for (int i = 0; i < _capacity; i++) {
        Bucket& bucket = _table[i];
        for (int j = 0; j < BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active && !bucket.entries[j].Deleted) {
                keys[index++] = bucket.entries[j].Key;
            }
        }
    }
}

#endif
