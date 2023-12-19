#include "SrcMain.h"
#include <string>
#include "RleFile.h"

void ProcessCommandArgs(int argc, const char* argv[])
{

    RleFile file;
    
    const std::string fileSrc = argv[argc - 1];
    if(argc == 2) {
        if (fileSrc.find(".rll") != std::string::npos) {
            file.CreateArchive(fileSrc);
        }
        else {
            file.ExtractArchive(fileSrc);
            
        }
    }
}
