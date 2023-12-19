#include "RleFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

void RleFile::CreateArchive(const std::string& source)
{
    // Requires <fstream>
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg(); // Save the size of the file
        memblock = new char[static_cast<unsigned int>(size)];
        file.seekg(0, std::ios::beg); // Seek back to start of file
        file.read(memblock, size);
        file.close();
        // File data is now in memblock array
        // (Do something with it here...)
        mRleData.Compress(memblock, size);
        // Make sure to clean up!
        delete[] memblock;
    }
    
    
    mHeader.mFileSize = size;
    mHeader.mFileNameLength = source.length();
    mHeader.mFileName = source;

    std::string filename = source + ".rl1";
    // Open the file for output, in binary mode, and overwrite an existing file
    std::ofstream arc(filename, std::ios::out|std::ios::binary|std::ios::trunc);
    if (arc.is_open())
    {
        // Use arc.write function to write data here
        arc.write(mHeader.mSig, 4);
        arc.write(reinterpret_cast<char*>(&(mHeader.mFileSize)), 4);
        arc.write(reinterpret_cast<char*>(&(mHeader.mFileNameLength)), 1);
        arc.write(mHeader.mFileName.c_str(), mHeader.mFileName.size());
        arc.write(mRleData.mData, mRleData.mSize);
    }
}

void RleFile::ExtractArchive(const std::string& source)
{
    
    // Requires <fstream>
    std::ifstream::pos_type size;
    char* memblock;
    
    char* toBeDecompressed;
    
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg(); // Save the size of the file
        memblock = new char[static_cast<unsigned int>(size)];
        file.seekg(0, std::ios::beg); // Seek back to start of file
        file.read(memblock, size);
        file.close();
        // File data is now in memblock array
        // (Do something with it here...)
        mHeader.mFileSize = *(reinterpret_cast<int*>(&memblock[4]));
        mHeader.mFileNameLength = *(reinterpret_cast<int*>(&memblock[8]));
        for(int i = 9; i < 9 + mHeader.mFileNameLength; i++) {
            mHeader.mFileName += memblock[i];
        }
        
        int compressedSize = static_cast<unsigned int>(size) - mHeader.mFileNameLength - 9;
        toBeDecompressed = new char[static_cast<unsigned int>(compressedSize)];
        int count = 0;
        
        for(int i = 9 + mHeader.mFileNameLength; i < size; i++) {
            toBeDecompressed[count] = memblock[i];
            count += 1;
        }
        mRleData.Decompress(toBeDecompressed, compressedSize, mHeader.mFileSize);
        delete[] toBeDecompressed;
        
        // Make sure to clean up!
        delete[] memblock;
    }
        
    // Open the file for output, in binary mode, and overwrite an existing file
    std::ofstream arc(mHeader.mFileName, std::ios::out|std::ios::binary|std::ios::trunc);
    if (arc.is_open())
    {
        // Use arc.write function to write data here
        arc.write(mRleData.mData, mRleData.mSize);
    }
}
