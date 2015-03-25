#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <msgpack.hpp>
#include <boost/timer/timer.hpp>

#include "MsgpackFilter.h"
#include "include/ast.hpp"
#include "include/expression.hpp"

#define NUM_ROWS 1000000

double uniformRandom()
{
    return (double)(rand())/(double)(RAND_MAX);
}

std::vector<std::string> Generate(int n) {

    std::vector<std::string> rows;
    rows.reserve(n);
    std::stringstream buffer;
    for (int i=0; i<n; i++) {
        msgpack::packer<std::stringstream> packer(buffer);
        packer.pack_map(2);
        packer.pack(std::string("lat"));
        packer.pack(uniformRandom());
        packer.pack(std::string("long"));
        packer.pack(uniformRandom());
        rows.push_back(buffer.str());
        buffer.str("");
        buffer.clear();
    }
    return rows;
}

int filterDataWithFilter(const std::vector<std::string> &input, double filterVal, const std::string &key) {
    boost::timer::cpu_timer timer;
    int result = 0;
    Ast ast("foo");
    Expression expr(ast);
    for (std::vector<std::string>::const_iterator it(input.begin()); it != input.end(); ++it) {
        const std::string& row = *it;
        if (expr.evaluate(row)) {
            result++;
        }
    }
    std::cout << timer.format();
    return result;
}

int main(int argc, const char* argv[]) {
    std::string stopper;
    std::vector<std::string> input = Generate(NUM_ROWS);
    std::cout << "Generated Data\n";
    double filterVal = uniformRandom();
    std::cout << "Filtered for long >=" << filterVal << "\n";
    std::cout << "With Filter\n";
    int objectCount = filterDataWithFilter(input, filterVal, "long");
    std::cout << "Filtered Data\n";
    std::cout << "For 1000000, expected number is 223881, actual was " << objectCount << " for " << NUM_ROWS << " rows\n";
}
