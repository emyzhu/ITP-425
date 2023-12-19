#pragma once
#include <string>
#include <utility>
#include <functional>

template <typename ValueType, 
	typename HashFuncType = std::hash<std::string>>
class HashTable
{
public:
	// Constructs the hash table with the given capacity (size of underlying array)
	HashTable(size_t capacity)
		:mTableData(new std::pair<std::string, ValueType>[capacity])
		,mCapacity(capacity)
		,mSize(0)
	{ }

    // copy constructor
    HashTable(const HashTable& rhs) :
        mTableData(new std::pair<std::string, ValueType>[rhs.mCapacity]),
        mCapacity(rhs.mCapacity),
        mSize(rhs.mSize)
    {
        std::copy( rhs.mTableData, rhs.mTableData + rhs.mCapacity, mTableData);
    }
    
    //assignment operator
    HashTable& operator=(const HashTable& rhs) {
        if(&rhs != this) {
            std::copy( rhs.mTableData, rhs.mTableData + rhs.mCapacity, mTableData);
            mCapacity = rhs.mCapacity;
            mSize = rhs.mSize;
        }
        return *this;
    }
    
    
    // move constructor
    HashTable(HashTable&& rhs):
        mTableData(std::move(rhs.mTableData)),
        mCapacity(std::move(rhs.mCapacity)),
        mSize(std::move(rhs.mSize))
    {
        rhs.mTableData = nullptr;
        rhs.mCapacity = 0;
        rhs.mSize = 0;
    }
    
    // move assignement operator
    HashTable& operator=(HashTable&& rhs) {
        if(&rhs != this) {
            mTableData = std::move(rhs.mTableData);
            mCapacity = std::move(rhs.mCapacity);
            mSize = std::move(rhs.mSize);
            
            rhs.mTableData = nullptr;
            rhs.mCapacity = 0;
            rhs.mSize = 0;
            
        }
        return *this;
    }
    
    // destructor
    ~HashTable()
    {
        mSize = 0;
        mCapacity = 0;
        mTableData = nullptr;
//        delete[] mTableData;
    }

	// Returns overall capacity of hash table (size of underlying array)
	size_t Capacity() const
	{
		return mCapacity;
	}

	// Returns the number of elements currently inserted in the hash table
	size_t Size() const
	{
		return mSize;
	}

	// Tries to insert (key, value) into hash table
	// Returns true if insert succeeds, false if not
	bool Insert(const std::string& key, const ValueType& value)
	{

                
        int hIndex = mHashFunc(key) % mCapacity;
        
        // Try index at hashFuncResult % capacity. If not empty, then find the next available index. If you reach the end and still no space, wrap around to index 0
        for (int i = 0; i < mCapacity; i++) {
            if (mTableData[(i + hIndex) % mCapacity].first.empty()) {
                mTableData[(i + hIndex) % mCapacity] = std::make_pair(key, value);
                mSize += 1;
                return true;
            }
        }
        
        // Returns true if successful, false otherwise
		return false;
	}

	// Returns pointer to value for given key, or nullptr
	// if not in hash table
	ValueType* Find(const std::string& key) const
	{
        //Finds the element, if it exists, using the same “linear probing” technique from Insert
        //If you encounter an empty index, then it’s not in the table
        //Don’t get stuck in an infinite loop
        int hIndex = mHashFunc(key) % mCapacity;
        

        for (int i = 0; i < mCapacity; i++) {
            if (mTableData[(i + hIndex) % mCapacity].first.empty()) {
                return nullptr;
            }
            if (mTableData[(i + hIndex) % mCapacity].first == key) {
                return &mTableData[(i + hIndex) % mCapacity].second;
            }
        }
        
        //Returns pointer to value if found, nullptr otherwise
		return nullptr;
	}

	// Executes provided lambda expression on each element inserted into the table
	void ForEach(std::function<void(const std::string& key, ValueType& value)> func)
	{
        for (int i = 0; i < mCapacity; i++) {
            if (!mTableData[i].first.empty()) {
                func(mTableData[i].first, mTableData[i].second);
            }
        }
	}

	// Gets a const pointer to underlying data (for testing)
	const std::pair<std::string, ValueType>* Data() const
	{
		return mTableData;
	}
private:
	// NOTE: you should not need any more member data!
	
	// Underlying hash table array
	std::pair<std::string, ValueType>* mTableData;
	// Allocated capacity of hash table array
	size_t mCapacity;
	// Number of elements in HashTable
	size_t mSize;
	// Hash function
	HashFuncType mHashFunc;
};
