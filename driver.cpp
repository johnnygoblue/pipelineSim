#include "PipelineSimulator.h"
#include <iostream>

void runTest(const std::string& name, const std::string& instructions,
             int r1, int r2, int r3, int r4) {
    std::cout << "\n=== Test: " << name << " ===\n";
    std::cout << "Initial registers: "
              << r1 << ", " << r2 << ", " << r3 << ", " << r4 << "\n";
    std::cout << "Instructions: " << instructions << "\n\n";

    PipelineSimulator sim(r1, r2, r3, r4);

    // Process instructions (space separated)
    size_t start = 0;
    size_t end = instructions.find(' ');
    while (end != std::string::npos) {
        sim.processInstruction(instructions.substr(start, end - start));
        start = end + 1;
        end = instructions.find(' ', start);
    }
    sim.processInstruction(instructions.substr(start));

    sim.runSimulation();
    std::cout << "=== End Test ===\n\n";
}

int main() {
    // Basic cases
    runTest("Single ADD", "ADD123", -99, 1, 9, 99);
    runTest("Single MUL", "MUL123", 2, 3, 4, 6);
    runTest("Single ST", "ST1", 1, 2, 3, 4);

    // Edge cases
    runTest("MUL Read takes priority over ADD Write", "ADD123 MUL234", -99, 2, 9, 50); // reads old value before write is applied
    runTest("Multiple Read works as intended", "ADD123 ST1", -99, 2, 9, 50); // both Read are the same and does not change value
    runTest("ST Read takes priority over ADD Write", "ADD123 ST1 ST3", -99, 2, 9, 50);  // ST3 prints 9, not -97
    runTest("Coincident Write, last Write wins", "MUL123 ADD243 ST3 ST3", -99, 2, 9, 50); // ADD comes after MUL so ADD Write wins, reg3 is 52 and outputs 9
    runTest("Consecutive Write", "ADD123 ADD243 ADD143", -99, 2, 9, 50); // value is written correctly each time
    runTest("README sample", "ADD123 MUL341 ST1", -99, 1, 4, 40);
    return 0;
}
