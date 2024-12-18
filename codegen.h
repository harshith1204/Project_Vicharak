#ifndef ASSEMBLY_GENERATOR_H
#define ASSEMBLY_GENERATOR_H

#include "parser.h"
#include <ostream>

class AssemblyGenerator {
public:
    explicit AssemblyGenerator(std::ostream& output) : output(output) {}
    void generate(const ASTNode& node);

private:
    std::ostream& output; 
};

#endif 
