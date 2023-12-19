# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
	SrcMain.h
    FASTAParser.h
    DNATranslator.h
    NeedlemanWunschAlgorithm.h
)

set(SOURCE_FILES
	SrcMain.cpp
    FASTAParser.cpp
    DNATranslator.cpp
    NeedlemanWunschAlgorithm.cpp
)

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})
