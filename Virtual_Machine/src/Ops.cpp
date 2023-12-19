#include "Ops.h"
#include "Machine.h"
#include<iostream>

void Exit::Execute(Machine& machine)
{
    machine.setFlag("exit", true);
}

void MovI::Execute(Machine& machine)
{
    machine.setRegister(std::get<0>(mParameters), std::get<1>(mParameters));
}

void Add::Execute(Machine & machine)
{

    try {
        int64_t check = static_cast<int64_t>(machine.getRegister(std::get<1>(mParameters))) + static_cast<int64_t>(machine.getRegister(std::get<2>(mParameters)));
        if (check > INT32_MAX) {
            machine.setRegister("ex", 12);
            throw std::invalid_argument("Integer overflow (Addition)");
        }
        if (check < INT32_MIN) {
            machine.setRegister("ex", 13);
            throw std::invalid_argument("Integer underflow (Addition)");
        }
    }
    catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    
    machine.setRegister(std::get<0>(mParameters), machine.getRegister(std::get<1>(mParameters)) + machine.getRegister(std::get<2>(mParameters)));
}

void Mov::Execute(Machine & machine)
{
    machine.setRegister(std::get<0>(mParameters), machine.getRegister(std::get<1>(mParameters)));
}

void Sub::Execute(Machine & machine)
{
    try {
        int64_t check = static_cast<int64_t>(machine.getRegister(std::get<1>(mParameters))) - static_cast<int64_t>(machine.getRegister(std::get<2>(mParameters)));
        if (check > INT32_MAX) {
            machine.setRegister("ex", 12);
            throw std::invalid_argument("Integer overflow (Subtraction)");
        }
        if (check < INT32_MIN) {
            machine.setRegister("ex", 13);
            throw std::invalid_argument("Integer underflow (Subtraction)");
        }
    }
    catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    
    machine.setRegister(std::get<0>(mParameters), machine.getRegister(std::get<1>(mParameters)) - machine.getRegister(std::get<2>(mParameters)));
    
}

void Mul::Execute(Machine & machine)
{
    try {
        int64_t check = static_cast<int64_t>(machine.getRegister(std::get<1>(mParameters))) * static_cast<int64_t>(machine.getRegister(std::get<2>(mParameters)));
        if (check > INT32_MAX) {
            machine.setRegister("ex", 12);
            throw std::invalid_argument("Integer overflow (Multiplication)");
        }
        if (check < INT32_MIN) {
            machine.setRegister("ex", 13);
            throw std::invalid_argument("Integer underflow (Multiplication)");
        }
    }
    catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    machine.setRegister(std::get<0>(mParameters), machine.getRegister(std::get<1>(mParameters)) * machine.getRegister(std::get<2>(mParameters)));
    
}

void Div::Execute(Machine & machine)
{
    if (machine.getRegister(std::get<2>(mParameters)) == 0) {
        machine.setRegister("ex", 102);
        machine.setFlag("exit", true);
        throw std::invalid_argument("Divide by 0");
    }
    try {
        int64_t check = static_cast<int64_t>(machine.getRegister(std::get<1>(mParameters))) / static_cast<int64_t>(machine.getRegister(std::get<2>(mParameters)));
        if (check > INT32_MAX) {
            machine.setRegister("ex", 12);
            throw std::invalid_argument("Integer overflow (division)");
        }
        if (check < INT32_MIN) {
            machine.setRegister("ex", 13);
            throw std::invalid_argument("Integer underflow (division)");
        }
    }
    catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    machine.setRegister(std::get<0>(mParameters), machine.getRegister(std::get<1>(mParameters)) / machine.getRegister(std::get<2>(mParameters)));
    
}

void Inc::Execute(Machine & machine)
{
    machine.setRegister(std::get<0>(mParameters), machine.getRegister(std::get<0>(mParameters)) + 1);
    
}

void Dec::Execute(Machine & machine)
{
    machine.setRegister(std::get<0>(mParameters), machine.getRegister(std::get<0>(mParameters)) - 1);
    
}

void Jmp::Execute(Machine & machine)
{
    machine.setRegister("pc", machine.getRegister(std::get<0>(mParameters)));
    
}

void Cmpeq::Execute(Machine & machine)
{
    machine.setFlag("test", machine.getRegister(std::get<0>(mParameters)) == machine.getRegister(std::get<1>(mParameters)));
    
}

void Cmplt::Execute(Machine & machine)
{
    machine.setFlag("test", machine.getRegister(std::get<0>(mParameters)) < machine.getRegister(std::get<1>(mParameters)));
    
}

void Jt::Execute(Machine & machine)
{
    if (machine.getFlag("test")) {
        machine.setRegister("pc", machine.getRegister(std::get<0>(mParameters)));
    }
}

void Jnt::Execute(Machine & machine)
{
    if (!machine.getFlag("test")) {
        machine.setRegister("pc", machine.getRegister(std::get<0>(mParameters)));
    }
}

void Push::Execute(Machine & machine)
{
    machine.pushOntoStack(machine.getRegister(std::get<0>(mParameters)));
    machine.setRegisterPushPop("sc",  machine.getRegister("sc") + 1);

}

void Pop::Execute(Machine & machine)
{
    machine.setRegister(std::get<0>(mParameters), machine.popFromStack());
    machine.setRegisterPushPop("sc",  machine.getRegister("sc") - 1);
}

void Load::Execute(Machine & machine)
{
    machine.setRegister(std::get<0>(mParameters), machine.accessStack(machine.getRegister(std::get<1>(mParameters))));
}

void Store::Execute(Machine & machine)
{
    machine.accessStack(machine.getRegister(std::get<0>(mParameters))) = machine.getRegister(std::get<1>(mParameters));
}

void LoadI::Execute(Machine & machine)
{
    machine.setRegister(std::get<0>(mParameters), machine.accessStack(std::get<1>(mParameters)));
}

void StoreI::Execute(Machine & machine)
{
    machine.accessStack(std::get<0>(mParameters)) = machine.getRegister(std::get<1>(mParameters));
}

void Loadsc::Execute(Machine & machine)
{
    machine.setRegister(std::get<0>(mParameters), machine.accessStack(machine.getRegister("sc") - machine.getRegister(std::get<1>(mParameters)) - 1));
}

void Storesc::Execute(Machine & machine)
{
    machine.accessStack(machine.getRegister("sc") - machine.getRegister(std::get<0>(mParameters)) - 1) = machine.getRegister(std::get<1>(mParameters));
}

void PenDown::Execute(Machine& machine)
{
    machine.setFlag("pen", true);
}

void PenUp::Execute(Machine& machine)
{
    machine.setFlag("pen", false);
}

void Fwd::Execute(Machine& machine)
{
    machine.implementFwd(machine.getRegister(std::get<0>(mParameters)));
}

void Back::Execute(Machine& machine)
{
    machine.implementFwd(machine.getRegister(std::get<0>(mParameters)) * -1);
}
