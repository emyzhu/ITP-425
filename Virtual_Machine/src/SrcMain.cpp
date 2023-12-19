#include "SrcMain.h"
#include <iostream>
#include "Machine.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO
    Machine mach;
    mach.addOp(argv[1]);
    mach.Execute(mach);
}
