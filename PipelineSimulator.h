#ifndef PIPELINE_SIMULATOR_H
#define PIPELINE_SIMULATOR_H

#include <list>
#include <string>
#include <vector>
#include <utility>

enum class InstructionType { ADD, MUL, ST };

enum class Stage { S1, S2, S3, S4, DONE };

struct Instruction {
    InstructionType type;
    int reg1;  // First source register (for ADD/MUL) or target register (for ST)
    int reg2;  // Second source register (for ADD/MUL), unused for ST
    int reg3;  // Destination register (for ADD/MUL), unused for ST
    Stage stage;
};

class PipelineSimulator {
public:
    PipelineSimulator(int r1, int r2, int r3, int r4);
    void processInstruction(const std::string& inst);
    void runSimulation();
    void printState() const;

private:
    int registers[4];
    std::list<int> active_inst;
    std::vector<Instruction> instructions;
    std::vector<int> completed_inst;
    int cycle;
    std::vector<std::pair<int, int>> registers_to_write;

    void removeCompletedInst();
    void simulateCycle();
    void handleInstructionOp(int inst_idx);
    void handleReadOperation(int inst_idx);
    void handleOutputOperation(int inst_idx);
    void handleWriteOperations(int inst_idx);
};

#endif
