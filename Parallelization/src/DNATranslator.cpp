#include "DNATranslator.h"
#include <string>
#include <iostream>
//create a state variable. increement state upon verifying with state machine depending on what table says about the letter residing in that state.


void DNATranslator::Translate() {
    int count = 0;
    for(char bp : mSequence) {
        count += 1;
        
        //read in base pair
        mBasePair = bp;
        
        int row = mState;
        int col;
        
        switch(bp) {
            case 'T':
                col = 0;
                break;
            case 'C':
                col = 1;
                break;
            case 'A':
                col = 2;
                break;
            case 'G':
                col = 3;
                break;
        }
        
        //determine and update state, then act according to updated state
        mState = mTable[row][col];
                
        // create AA and increase count
        if(isalpha(mState)) {
        
            char aminoAcid = mState;
            
            auto foundAminoAcid = std::find_if(mAminoAcidCount.begin(), mAminoAcidCount.end(),
                [&aminoAcid](const std::pair<std::string, int>& aa){
                return aa.first[1] == aminoAcid;
            });
            
            foundAminoAcid->second += 1;
            
            mAminoAcidTotal += 1;
            
            mState = 3;
            
        }
                            
    }
}
