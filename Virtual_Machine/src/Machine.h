#pragma once
#include "Ops.h"
#include "../img/bitmap_image.hpp"
#include <memory>
#include <vector>
#include <map>

template <typename T>
std::shared_ptr<Op> CreateOp();

class Machine
{
public:
        
    
    Machine();
    
    void addOp(const std::string& fileName);
    
    int getRegister(const std::string&registerKey);
    void setRegister(const std::string& registerKey, int val);
    bool getFlag(const std::string& flagKey);
    void setFlag(const std::string& flagKey, bool val);
    
    void setRegisterPushPop(const std::string& registerKey, int val);

    
    void Execute(Machine& mach);
    
    void pushOntoStack(int val);
    int popFromStack();
    int& accessStack(int index);
    
    void implementFwd(int reg1);
    
    void changeColor(int tcVal);
    
private:
    std::map<std::string, std::function<std::shared_ptr<Op>()>> opMap;
    
	std::vector<std::shared_ptr<Op>> mOps;
    std::map<std::string, int> mRegisters = {{"r0", 0}, {"r1", 0}, {"r2", 0}, {"r3", 0}, {"r4", 0}, {"r5", 0}, {"r6", 0}, {"r7", 0},{"pc", 0}, {"sc", 0}, {"ex", 0}, {"tx", 0}, {"ty", 0}, {"tr", 0}, {"tc", 0}};
    std::map<std::string, bool> mFlags = {{"exit", false}, {"test", false}, {"pen", false}};
    std::vector<int> mStack;
    bitmap_image mImg;
    image_drawer mImgDrawer;

};
