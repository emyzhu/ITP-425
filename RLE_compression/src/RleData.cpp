#include "RleData.h"
#include <vector>
#include <iostream>

void RleData::Compress(const char* input, size_t inSize)
{
    delete[] mData;
    mSize = 0;
    
    mData = new char [inSize * 2];
    
    //run length is default 1 in the case of one character
    int runLen = 1;
    
    std::vector<char> temp;
    
    // if the length of the input is 1
    if(inSize == 1) {
        mSize += 1;
        mData[mSize - 1] = runLen;
        mSize += 1;
        mData[mSize - 1] = input[0];
    }

    //loop through input array
    for(size_t i = 1; i < inSize; i++) {
        char curr = input[i];
        char prev = input[i - 1];
        
        if (curr == prev) {
            //long run
            if (runLen == MaxRunSize()) {
                temp.push_back(runLen);
                temp.push_back(prev);
                runLen = 0;
            }
            runLen += 1;
        }
        else {
            temp.push_back(runLen);
            temp.push_back(prev);
            runLen = 1;
        }
        
        if(i == inSize - 1) {
            temp.push_back(runLen);
            temp.push_back(curr);
        }
    } // end of for loop
    
    int numOfSizeOne = 0;
    std::vector<char> negRun;
    //add vector elements into array
    for (auto i = temp.begin(); i != temp.end(); ++i) {
        int index = i - temp.begin();
        
        //if at an even index, there is a 1, increment counter by 1
        if (index % 2 == 0 && temp[index] == 1) {
            //long negative run
            if (numOfSizeOne == MaxRunSize()) {
                mSize += 1;
                int negRunSize = negRun.size();
                if (negRun.size() > 1) {
                    negRunSize = negRun.size() * -1;
                }
                mData[mSize - 1] = negRunSize;
                for (auto i = negRun.begin(); i != negRun.end(); ++i) {
                    mSize += 1;
                    mData[mSize - 1] = *i;
                }
                negRun.clear();
                numOfSizeOne = 0;
            }
            numOfSizeOne += 1;
        }
        //if the previous value at an odd index was a 1, add the current value to the negative run vector
        else if ((index - 1) % 2 == 0 && temp[index - 1] == 1) {
            negRun.push_back(*i);
            
            //accounting for the end of a negative run
            if (temp[index + 1] != 1) {
                mSize += 1;
                int negRunSize = negRun.size();
                if (negRun.size() > 1) {
                    negRunSize = negRun.size() * -1;
                }
                mData[mSize - 1] = negRunSize;
                for (auto i = negRun.begin(); i != negRun.end(); ++i) {
                    mSize += 1;
                    mData[mSize - 1] = *i;
                }
                negRun.clear();
                numOfSizeOne = 0;
            }
        }
        //else a positive run
        else {
            mSize += 1;
            mData[mSize - 1] = *i;
        }
    }
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
    delete[] mData;
    mSize = 0;
    
    mData = new char [outSize];
    
    int indexOfmData = 0;
    int timesToAdd = 0;
    bool isInfo = true;
    int whereToStop;
    
    for(size_t i = 0; i < inSize; i++) {
        if (isInfo) {
            if (input[i] > 0) {
                timesToAdd = input[i];
            }
            else {
                whereToStop = std::abs(input[i]) + indexOfmData;
            }
            isInfo = false;
        }
        else {
            if(timesToAdd > 0) {
                for (int j = 0; j < timesToAdd; j++) {
                    mSize += 1;
                    mData[indexOfmData] = input[i];
                    indexOfmData += 1;
                }
                timesToAdd = 0;
                isInfo = true;
            }
            else {
                mSize += 1;
                mData[indexOfmData] = input[i];
                indexOfmData += 1;
                
                if (indexOfmData == whereToStop) {
                    isInfo = true;
                }
                
            }
        }
    }
    
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}
