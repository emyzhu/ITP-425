#ifndef NeedlemanWunschAlgorithm_h
#define NeedlemanWunschAlgorithm_h


#endif /* NeedlemanWunschAlgorithm_h */
#pragma once
#include <string>
#include <FASTAParser.h>
#include <vector>
#include <fstream>


class NeedlemanWunschAlgorithm {
    
public:
    
    FASTAParser constructSequence(std::string& file) {
        FASTAParser p(file);
        p.parseFile();
        return p;
    }
    //The constructor will load in the two input FASTA files and make sure everything is okay with them

    
    FASTAParser mParsedA;
    FASTAParser mParsedB;

    std::pair<std::string, std::string> mResultingSequence;
    
    int mFinalScore;
    
    
    
    NeedlemanWunschAlgorithm(std::string file1, std::string file2)
    : mParsedA(constructSequence(file1))
    , mParsedB(constructSequence(file2))
    , mResultingSequence(std::make_pair("", ""))
    , mFinalScore(0)
    { }
    
    // A function that populates the score/direction grid, runs the algorithm, and produces the resultant sequence strings.
    enum Direction : char
    {
        aboveLeft = 'A',
        Left = 'L',
        Up = 'U'
    };
    
    
    int mMatchScore = 1;
    int mMismatchScore = -1;
    int mGapScore = -1;
    
    inline const std::pair<std::string, std::string>& NMAlgorithm() {
        
        
        std::vector<std::vector<int>> score((mParsedB.mSequence.length() + 1), std::vector<int>(mParsedA.mSequence.length() + 1));
        std::vector<std::vector<char>> direction((mParsedB.mSequence.length() + 1), std::vector<char>(mParsedA.mSequence.length() + 1));
        
        for(int i = 1; i < mParsedA.mSequence.length() + 1; ++i) {
            score [0][i] = i * -1;
            direction [0][i] = Left;
        }
        for (int i = 1; i < mParsedB.mSequence.length() + 1; ++i) {
            score [i][0] = i * -1;
            direction [i][0] = Up;
        }
        
        for ( int i = 1; i < mParsedB.mSequence.length() + 1; ++i) {
            for (int j = 1; j < mParsedA.mSequence.length() + 1; ++j) {
                 
//                int max;
//                if (mParsedA.mSequence[j - 1] == mParsedB.mSequence[i - 1]) {
//                    score[i][j] = score[i - 1][j - 1] + mMatchScore;
//                    direction[i][j] = aboveLeft;
//
//                } else {
//                    score[i][j] = score[i - 1][j - 1] + mMismatchScore;
//                    direction[i][j] = aboveLeft;
//
//                }
//
//                if(score[i][j - 1] + mGapScore > score[i][j]) {
//                    score[i][j] = score[i][j - 1] + mGapScore;
//                    direction[i][j] = Left;
//
//                }
//                if(score[i - 1][j] > score[i][j]) {
//                    score[i][j] = score[i - 1][j] + mGapScore;
//                    direction[i][j] = Up;
//                }
                int upLeft;
                if (mParsedA.mSequence[j - 1] == mParsedB.mSequence[i - 1]) {
                    upLeft = score[i - 1][j - 1] + mMatchScore;

                } else {
                    upLeft = score[i - 1][j - 1] + mMismatchScore;

                }
                int left = score[i][j - 1] + mGapScore;

                int right = score[i - 1][j] + mGapScore;

                int max = std::max(upLeft, left);
                max = std::max(max, right);

                if (max == upLeft) {
                    direction[i][j] = aboveLeft;
                }
                else if(max == left) {
                    direction[i][j] = Left;
                }
                else {
                    direction[i][j] = Up;
                }
                
                score[i][j] = max;

            }
        }
        //row, col
        std::pair<int, int> location = std::make_pair(mParsedB.mSequence.length(), mParsedA.mSequence.length());
        
        mFinalScore = score[mParsedB.mSequence.length()][mParsedA.mSequence.length()];

        while(!(location.first == 0 && location.second == 0)) {
            
            char d = direction[location.first][location.second];
            
            switch (d) {
                case 'A':
                    mResultingSequence.first += mParsedA.mSequence[location.second - 1];
                    mResultingSequence.second += mParsedB.mSequence[location.first - 1];
                    
                    location.first -= 1;
                    location.second -= 1;
                    break;
                case 'L':
                    mResultingSequence.first += mParsedA.mSequence[location.second - 1];
                    mResultingSequence.second += "_";
                    
                    location.second -= 1;
                    break;
                default:
                    mResultingSequence.first += "_";
                    mResultingSequence.second += mParsedB.mSequence[location.first - 1];
                    
                    location.first -= 1;
                    break;
            }
        }

        std::reverse(mResultingSequence.first.begin(), mResultingSequence.first.end());
        std::reverse(mResultingSequence.second.begin(), mResultingSequence.second.end());
                
        return mResultingSequence;
    };
    
    const void outputMatch(std::pair<std::string, std::string>& match);
};
