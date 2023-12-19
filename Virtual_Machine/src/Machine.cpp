#include "Machine.h"
#include <fstream>
#include <sstream>

template <typename T>
std::shared_ptr<Op> CreateOp()
{
    return std::make_shared<T>();
}

Machine::Machine()
: mImg(320,240)
, mImgDrawer(mImg)
{
    mImg.set_all_channels(0,0,0);
    mImgDrawer.pen_width(1);
    mImgDrawer.pen_color(255, 255, 255);
    
    opMap.emplace("exit", &CreateOp<Exit>);
    
    opMap.emplace("movi", &CreateOp<MovI>);
    opMap.emplace("add", &CreateOp<Add>);
    
    opMap.emplace("mov", &CreateOp<Mov>);
    opMap.emplace("sub", &CreateOp<Sub>);
    opMap.emplace("mul", &CreateOp<Mul>);
    opMap.emplace("div", &CreateOp<Div>);
    opMap.emplace("inc", &CreateOp<Inc>);
    opMap.emplace("dec", &CreateOp<Dec>);
    
    opMap.emplace("jmp", &CreateOp<Jmp>);
    opMap.emplace("cmpeq", &CreateOp<Cmpeq>);
    opMap.emplace("cmplt", &CreateOp<Cmplt>);
    opMap.emplace("jt", &CreateOp<Jt>);
    opMap.emplace("jnt", &CreateOp<Jnt>);
    
    opMap.emplace("push", &CreateOp<Push>);
    opMap.emplace("pop", &CreateOp<Pop>);
    opMap.emplace("load", &CreateOp<Load>);
    opMap.emplace("store", &CreateOp<Store>);
    opMap.emplace("loadi", &CreateOp<LoadI>);
    opMap.emplace("storei", &CreateOp<StoreI>);
    opMap.emplace("loadsc", &CreateOp<Loadsc>);
    opMap.emplace("storesc", &CreateOp<Storesc>);

    opMap.emplace("pendown", &CreateOp<PenDown>);
    opMap.emplace("penup", &CreateOp<PenUp>);

    opMap.emplace("fwd", &CreateOp<Fwd>);
    opMap.emplace("back", &CreateOp<Back>);
}

void Machine::addOp(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string line;
    
    if (file.is_open()) {
        while (getline (file, line)) {
            // Get the string for op name and params
            std::string opName;
            std::string params;
            
            std::stringstream ss(line);
            
            std::getline(ss, opName, ' ');
            std::getline(ss, params, ' ');
            
            // Look up the opName in our map, and call the correct CreateOp function! (note the extra parenthesis at the end, that's the function call)
            std::shared_ptr<Op> ptr = opMap.at(opName)();
            ptr->Parse(params);
            
            mOps.emplace_back(ptr);
        }
        file.close();
    }
}

int Machine::getRegister(const std::string& registerKey) {
    return mRegisters.find(registerKey)->second;
}

void Machine::setRegisterPushPop(const std::string& registerKey, int val) {

    mRegisters[registerKey] = val;
}


void Machine::setRegister(const std::string& registerKey, int val) {
    if (registerKey == "sc") {
        setRegister("ex", 10);
        throw std::invalid_argument("Invalid write to the sc register");
    }
    if (registerKey == "r0") {
        setRegister("ex", 11);
        throw std::invalid_argument("Invalid write to the r0 register");
    }
    if (registerKey == "tc" && (val < 0 || val > 7)) {
        setRegister("ex", 14);
        throw std::invalid_argument("Invalid color");
    }
    mRegisters[registerKey] = val;
}

bool Machine::getFlag(const std::string& flagKey) {
    return mFlags.find(flagKey)->second;
}

void Machine::setFlag(const std::string& flagKey, bool val) {
    mFlags[flagKey] = val;
}

void Machine::Execute(Machine& mach) {
    
    std::ofstream outFile("log.txt");
    std::ofstream outFile2("stack.txt");

    if (outFile.is_open() && outFile2.is_open()) {

        
        int pcValue = 0;
        while(!mach.mFlags.at("exit")) {
            outFile << "********************" << std::endl;
            outFile << "r0=" << mRegisters["r0"] << std::endl;
            outFile << "r1=" << mRegisters["r1"] << std::endl;
            outFile << "r2=" << mRegisters["r2"] << std::endl;
            outFile << "r3=" << mRegisters["r3"] << std::endl;
            outFile << "r4=" << mRegisters["r4"] << std::endl;
            outFile << "r5=" << mRegisters["r5"] << std::endl;
            outFile << "r6=" << mRegisters["r6"] << std::endl;
            outFile << "r7=" << mRegisters["r7"] << std::endl;
            outFile << "pc=" << mRegisters["pc"] << std::endl;
            outFile << "sc=" << mRegisters["sc"] << std::endl;
            outFile << "ex=" << mRegisters["ex"] << std::endl;
            outFile << "tx=" << mRegisters["tx"] << std::endl;
            outFile << "ty=" << mRegisters["ty"] << std::endl;
            outFile << "tr=" << mRegisters["tr"] << std::endl;
            outFile << "tc=" << mRegisters["tc"] << std::endl;
            outFile << "exit=" << mFlags["exit"] << std::endl;
            outFile << "test=" << mFlags["test"] << std::endl;
            outFile << "pen=" << mFlags["pen"] << std::endl;
            
            outFile2 << "pc: " << getRegister("pc") << " Stack: ";
            for (int i = 0; i < mStack.size(); i++) {
                outFile2 << mStack[i] << " ";
            }
            outFile2 << "\n";
            
            pcValue = getRegister("pc");
            std::shared_ptr<Op> pcPtr = mOps[pcValue];
            pcValue += 1;
            setRegister("pc", pcValue);
            
            int tcBefore = mRegisters["tc"];
            
            try
            {
                pcPtr->Execute(mach);
            }
            catch (std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
                
            
            outFile << "Executing: " << pcPtr->GetName() << std::endl;
            
            if(tcBefore != mRegisters["tc"]) {
                try {
                    changeColor(mRegisters["tc"]);
                }
                catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            
        }
        
        outFile << "********************" << std::endl;
        outFile << "r0=" << mRegisters["r0"] << std::endl;
        outFile << "r1=" << mRegisters["r1"] << std::endl;
        outFile << "r2=" << mRegisters["r2"] << std::endl;
        outFile << "r3=" << mRegisters["r3"] << std::endl;
        outFile << "r4=" << mRegisters["r4"] << std::endl;
        outFile << "r5=" << mRegisters["r5"] << std::endl;
        outFile << "r6=" << mRegisters["r6"] << std::endl;
        outFile << "r7=" << mRegisters["r7"] << std::endl;
        outFile << "pc=" << mRegisters["pc"] << std::endl;
        outFile << "sc=" << mRegisters["sc"] << std::endl;
        outFile << "ex=" << mRegisters["ex"] << std::endl;
        outFile << "tx=" << mRegisters["tx"] << std::endl;
        outFile << "ty=" << mRegisters["ty"] << std::endl;
        outFile << "tr=" << mRegisters["tr"] << std::endl;
        outFile << "tc=" << mRegisters["tc"] << std::endl;
        outFile << "exit=" << mFlags["exit"] << std::endl;
        outFile << "test=" << mFlags["test"] << std::endl;
        outFile << "pen=" << mFlags["pen"] << std::endl;

        outFile2 << "pc: " << getRegister("pc") << " Stack: ";
        for (int i = 0; i < mStack.size(); i++) {
            outFile2 << mStack[i] << " ";
        }
        
        outFile.close();
        outFile2.close();
    }
    mImg.save_image("output.bmp");
}

void Machine::pushOntoStack(int val) {
    if(mStack.size() >= 256) {
        setRegister("ex", 101);
        setFlag("exit", true);
        throw std::invalid_argument("Stack overflow");
    }
    mStack.emplace_back(val);
}

int Machine::popFromStack() {
    if (getRegister("sc") == 0) {
        setRegister("ex", 103);
        setFlag("exit", true);
        throw std::invalid_argument("Pop when stack is empty");
    }
    int top = mStack.back();
    mStack.pop_back();
    return top;
}

int& Machine::accessStack(int index) {
    if(index < 0 || index >= getRegister("sc")) {
        setRegister("ex", 100);
        setFlag("exit", true);
        throw std::invalid_argument("Access violation");
    }
    return mStack[index];
}

void Machine::implementFwd(int reg1) {
    int tx = getRegister("tx");
    int ty = getRegister("ty");

    float angle = getRegister("tr") *  0.0174533f;
    
    int endx = tx + static_cast<int>(std::cos(angle) * reg1);
    int endy = ty + static_cast<int>(std::sin(angle) * reg1);

    if (getFlag("pen")) {
        mImgDrawer.line_segment(tx, ty, endx, endy);
    }

    setRegister("tx", endx);
    setRegister("ty", endy);
}

void Machine::changeColor(int tcVal) {
    switch (tcVal) {
        case 0:
            mImgDrawer.pen_color(255, 255, 255);
            break;
        case 1:
            mImgDrawer.pen_color(255, 255, 0);
            break;
        case 2:
            mImgDrawer.pen_color(255, 0, 255);
            break;
        case 3:
            mImgDrawer.pen_color(255, 0, 0);
            break;
        case 4:
            mImgDrawer.pen_color(0, 255, 255);
            break;
        case 5:
            mImgDrawer.pen_color(0, 255, 0);
            break;
        case 6:
            mImgDrawer.pen_color(0, 0, 255);
            break;
        case 7:
            mImgDrawer.pen_color(0, 0, 0);
            break;

    }
}


