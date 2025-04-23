#include "PipelineSimulator.h"
#include <iostream>

PipelineSimulator::PipelineSimulator(int r1, int r2, int r3, int r4) : cycle(0) {
    registers[0] = r1;
    registers[1] = r2;
    registers[2] = r3;
    registers[3] = r4;
}

void PipelineSimulator::processInstruction(const std::string& inst) {
    Instruction i;
    if (inst.substr(0, 3) == "ADD") {
        i.type = InstructionType::ADD;
        i.reg1 = inst[3] - '1';
        i.reg2 = inst[4] - '1';
        i.reg3 = inst[5] - '1';
    } else if (inst.substr(0, 3) == "MUL") {
        i.type = InstructionType::MUL;
        i.reg1 = inst[3] - '1';
        i.reg2 = inst[4] - '1';
        i.reg3 = inst[5] - '1';
    } else if (inst.substr(0, 2) == "ST") {
        i.type = InstructionType::ST;
        i.reg1 = inst[2] - '1';
    }
    i.stage = Stage::S1;
    instructions.push_back(i);
}

void PipelineSimulator::runSimulation() {
    while (cycle < instructions.size() || !active_inst.empty()) {
        if (cycle < instructions.size()) {
            active_inst.push_back(cycle);
        }
        simulateCycle();
        cycle++;
        printState();
    }
}

void PipelineSimulator::printState() const {
    std::cout << "Cycle " << cycle << ":\n";
    for (int i = 0; i < 4; ++i) {
        std::cout << "reg" << (i+1) << ": " << registers[i] << "  ";
    }
    std::cout << "\n-----------------\n";
}

void PipelineSimulator::removeCompletedInst() {
    for (int i : completed_inst) {
        active_inst.remove(i);
    }
    completed_inst.clear();
}

void PipelineSimulator::simulateCycle() {
    // Handle all operations except WRITE
    for (auto i : active_inst) {
        handleInstructionOp(i);
    }
    // Handle Write operation after all operations done
    for (auto i : active_inst) {
        handleWriteOperations(i);
    }

    removeCompletedInst();
}

void PipelineSimulator::handleInstructionOp(int inst_idx) {
    if (instructions[inst_idx].stage == Stage::DONE) {
        completed_inst.push_back(inst_idx); // add inst to be removed
        return;
    }
    switch(instructions[inst_idx].type) {
        case InstructionType::ADD:
            switch (instructions[inst_idx].stage) {
                case Stage::S1:
                    // does nothing
                    instructions[inst_idx].stage = Stage::S2;
                    break;
                case Stage::S2:
                    handleReadOperation(inst_idx);
                    instructions[inst_idx].stage = Stage::S3;
                    break;
                case Stage::S3:
                    int val1 = instructions[inst_idx].reg1;
                    int val2 = instructions[inst_idx].reg2;
                    int dest = instructions[inst_idx].reg3;
                    int result = val1 + val2;
                    registers_to_write.push_back( {dest, result} );
                    instructions[inst_idx].stage = Stage::DONE;
                    break;
            }
            break;
        case InstructionType::MUL:
            switch (instructions[inst_idx].stage) {
                case Stage::S1:
                    // does nothing
                    instructions[inst_idx].stage = Stage::S2;
                    break;
                case Stage::S2:
                    handleReadOperation(inst_idx);
                    instructions[inst_idx].stage = Stage::S3;
                    break;
                case Stage::S3:
                    // does nothing
                    instructions[inst_idx].stage = Stage::S4;
                    break;
                case Stage::S4:
                    int val1 = instructions[inst_idx].reg1;
                    int val2 = instructions[inst_idx].reg2;
                    int dest = instructions[inst_idx].reg3;
                    int result = val1 * val2;
                    registers_to_write.push_back( {dest, result} );
                    instructions[inst_idx].stage = Stage::DONE;
                    break;
            }
            break;
        case InstructionType::ST:
            switch (instructions[inst_idx].stage) {
                case Stage::S1:
                    handleReadOperation(inst_idx);
                    instructions[inst_idx].stage = Stage::S2;
                    break;
                case Stage::S2:
                    handleOutputOperation(inst_idx);
                    instructions[inst_idx].stage = Stage::DONE;
                    break;
            }
            break;
        default:
            // Should never end up here, may add exception handling if desired
            break;
    }
}

void PipelineSimulator::handleReadOperation(int inst_idx) {
    if (instructions[inst_idx].type == InstructionType::ADD ||
        instructions[inst_idx].type == InstructionType::MUL) {
        instructions[inst_idx].reg1 = registers[instructions[inst_idx].reg1];
        instructions[inst_idx].reg2 = registers[instructions[inst_idx].reg2];
    } else { // instructions[inst_idx].type == InstructionType::ST
        instructions[inst_idx].reg1 = registers[instructions[inst_idx].reg1];
    }
}

void PipelineSimulator::handleOutputOperation(int inst_idx) {
    if (instructions[inst_idx].type == InstructionType::ST &&
        instructions[inst_idx].stage == Stage::S2) {
        std::cout << instructions[inst_idx].reg1 << " ";
        instructions[inst_idx].stage == Stage::DONE;
    }
}

void PipelineSimulator::handleWriteOperations(int inst_idx) {
    for (auto itr = registers_to_write.begin(); itr != registers_to_write.end(); ++itr) {
        registers[itr->first] = itr->second;
    }
    registers_to_write.clear(); // clear after write is done
}
