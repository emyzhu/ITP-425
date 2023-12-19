#ifndef DNATranslator_h
#define DNATranslator_h


#endif /* DNATranslator_h */
#pragma once
#include <string>
#include <vector>
#include <utility>

class DNATranslator {
    
public:
    std::string mSequence;
    //stop: tag tga taa
    // T C A G
    int mTable [24][4] = {{0, 0, 1, 0} //A in start codon
                        ,{2, 0, 1, 0} //T in start codon
                        ,{0, 0, 1, 'M'} //G in start codon
                        //3
                        ,{4,9,14,19} //First letter of next amino acid
                        //4
                        ,{5, 6, 7, 8} //T**
                        ,{'F','F','L','L'} //TT*
                        ,{'S','S','S','S'} //TC*
                        ,{'Y','Y', 0, 0} //TA*
                        ,{'C','C', 0, 'W'} //TG*
                        //9
                        ,{10, 11, 12, 13} //C**
                        ,{'L','L','L','L'} //CT*
                        ,{'P','P','P','P'} //CC*
                        ,{'H','H','Q','Q'} //CA*
                        ,{'R','R','R','R'} //CG*
                        //14
                        ,{15, 16, 17, 18} //A**
                        ,{'I','I','I','M'} //AT*
                        ,{'T','T','T','T'} //AC*
                        ,{'N','N','K','K'} //AA*
                        ,{'S','S','R','R'} //AG*
                        //19
                        ,{20, 21, 22, 23} //G**
                        ,{'V','V','V','V'} //GT*
                        ,{'A','A','A','A'} //GC*
                        ,{'D','D','E','E'} //GA*
                        ,{'G','G','G','G'} //GG*
    };
    
    std::vector<std::pair<std::string, int>> mAminoAcidCount { {"(A) Alanine", 0}, {"(C) Cysteine", 0}, {"(D) Aspartic acid", 0}, {"(E) Glutamic acid", 0}, {"(F) Phenylalanine", 0}, {"(G) Glycine", 0}, {"(H) Histidine", 0}, {"(I) Isoleucine", 0}, {"(K) Lysine", 0}, {"(L) Leucine", 0}, {"(M) Methionine", 0}, {"(N) Asparagine", 0}, {"(P) Proline", 0}, {"(Q) Glutamine", 0}, {"(R) Arginine", 0}, {"(S) Serine", 0}, {"(T) Threonine", 0}, {"(V) Valine", 0}, {"(W) Tryptophan", 0}, {"(Y) Tyrosine", 0} };
    
    char mBasePair;
    int mState;
    int mAminoAcidTotal;
    
    
    
    DNATranslator(std::string seq)
    : mSequence(seq)
    ,mBasePair(0)
    ,mState(0)
    ,mAminoAcidTotal(0)
    { }
    
    void Translate();
};

