#include <NeedlemanWunschAlgorithm.h>
#include <iostream>

//std::pair<std::string, std::string> NeedlemanWunschAlgorithm::NMAlgorithm() {
//    
//    std::vector<std::vector<int>> score((mParsedB.mSequence.length() + 1), std::vector<int>(mParsedA.mSequence.length() + 1));
//    std::vector<std::vector<char>> direction((mParsedB.mSequence.length() + 1), std::vector<char>(mParsedA.mSequence.length() + 1));
//    
//    for(int i = 1; i < mParsedA.mSequence.length() + 1; ++i) {
//        score [0][i] = i * -1;
//        direction [0][i] = Left;
//    }
//    for (int i = 1; i < mParsedB.mSequence.length() + 1; ++i) {
//        score [i][0] = i * -1;
//        direction [i][0] = Up;
//    }
//    
//    std::string resultingSequenceA;
//    std::string resultingSequenceB;
//        
//    for ( int i = 1; i < mParsedB.mSequence.length() + 1; ++i) {
//        for (int j = 1; j < mParsedA.mSequence.length() + 1; ++j) {
//            
//            std::vector<std::pair<int, char>> allScores;
//            
//            if (mParsedA.mSequence[j - 1] == mParsedB.mSequence[i - 1]) {
//                allScores.emplace_back(std::make_pair(score[i - 1][j - 1] + mMatchScore, aboveLeft));
//            } else {
//                allScores.emplace_back(std::make_pair(score[i - 1][j - 1] + mMismatchScore, aboveLeft));
//            }
//            allScores.emplace_back(std::make_pair(score[i][j - 1] + mGapScore, Left));
//            allScores.emplace_back(std::make_pair(score[i - 1][j] + mGapScore, Up));
//            
//            std::sort(allScores.begin(), allScores.end(), [](const std::pair<int,char> &left, const std::pair<int,char> &right) {
//                return left.first > right.first;
//            });
//            
//            std::pair<int, char>  highestScore = allScores[0];
//
//
//            score[i][j] = highestScore.first;
//            direction[i][j] = highestScore.second;
//        }
//    }
//    //row, col
//    std::pair<int, int> location = std::make_pair(mParsedB.mSequence.length(), mParsedA.mSequence.length());
//    
//    mFinalScore = score[mParsedB.mSequence.length()][mParsedA.mSequence.length()];
//
//    while(!(location.first == 0 && location.second == 0)) {
//        
//        char d = direction[location.first][location.second];
//        
//        switch (d) {
//            case 'A':
//                resultingSequenceA += mParsedA.mSequence[location.second - 1];
//                resultingSequenceB += mParsedB.mSequence[location.first - 1];
//                
//                location.first -= 1;
//                location.second -= 1;
//                break;
//            case 'L':
//                resultingSequenceA += mParsedA.mSequence[location.second - 1];
//                resultingSequenceB += "_";
//                
//                location.second -= 1;
//                break;
//            default:
//                resultingSequenceA += "_";
//                resultingSequenceB += mParsedB.mSequence[location.first - 1];
//                
//                location.first -= 1;
//                break;
//        }
//    }
//
//    std::reverse(resultingSequenceA.begin(), resultingSequenceA.end());
//    std::reverse(resultingSequenceB.begin(), resultingSequenceB.end());
//    
//    return std::make_pair(resultingSequenceA,resultingSequenceB);
//}

const void NeedlemanWunschAlgorithm::outputMatch(std::pair<std::string, std::string>& match) {
    std::ofstream outFile ("match.txt");
    if (outFile.is_open()) {
        
        outFile << "A: " << mParsedA.mHeader << std::endl;
        outFile << "B: " << mParsedB.mHeader << std::endl;
        
        outFile << "Score: " << mFinalScore << "\n" << std::endl;
        
        int count = match.first.length();
        
        for(int t = 0; t < count; t += 70) {
            
            for (int i = 0; i < 70 && i + t != count; ++i) {
                outFile << match.first[i + t];
                if ((i % 69 == 0 && i > 0) || t + i == count - 1) {
                   outFile << "\n";
                }
            }
            for (int i = 0; i < 70 && i + t != count; ++i) {
                if (match.first[i + t] == match.second[i + t]) {
                    outFile << "|";
                }
                else {
                    outFile << " ";
                }
                if ((i % 69 == 0 && i > 0) || t + i == count - 1) {
                   outFile << "\n";
                }
            }
            for (int i = 0; i < 70 && i + t != count; ++i) {
                outFile << match.second[i + t];
                if ((i % 69 == 0 && i > 0) || t + i == count - 1) {
                   outFile << "\n\n";
                }
            }
        }
    }
        outFile.close();
}

