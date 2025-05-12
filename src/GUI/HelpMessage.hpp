#ifndef HELPMESSAGE_H
#define HELPMESSAGE_H

#include <iostream>

class HelpMessage {
public:
    static void printHelp() {
        std::cout <<
        "Usage:\n"
        "  FILE INPUT MODE:\n"
        "    ./ProjectPath --file <algorithmFlag> <typeFlag> <inputFile> <outputFile>\n"
        "      <algorithmFlag> : --is for Insertion Sort, --bi for Binary Insertion Sort, --hs for Heap Sort, --qs for Quick Sort"
        "      <typeFlag>      : --i for int, --f for float, --d for double\n"
        "      <inputFile>     : Input file containing the data to be sorted\n"
        "      <outputFile>    : Output file for the sorted data\n\n"
        "  BENCHMARK MODE:\n"
        "     ./ProjectPath --benchmark <algorithmFlag> <typeFlag> <size> [minValue maxValue] <outputFile>\n"
        "      <algorithmFlag> : --is for Insertion Sort, --bi for Binary Insertion Sort, --hs for Heap Sort, --qs for Quick Sort"
        "      <typeFlag>      : --i for int, --f for float, --d for double\n"
        "      <size>          : Number of elements to generate\n"
        "      [minValue maxValue] : (Optional) Range for generating random numbers (default: 0 and 1000)\n"
        "      <outputFile>    : File where benchmark result (sorted output) will be saved\n\n"
        "  SERIES MODE:\n"
        "    ./ProjectPath --series <algorithmFlag> <typeFlag> <size> <repetitionCount> [minValue maxValue] <outputFile> <individualOutputFlag>\n"
        "      <algorithmFlag>     : --is for Insertion Sort, --bi for Binary Insertion Sort, --hs for Heap Sort, --qs for Quick Sort\n"
        "      <typeFlag>          : --i for int, --f for float, --d for double\n"
        "      <size>       : Number of elements to generate for each test run\n"
        "      <repetitionCount>  : Number of test repetitions to run the benchmark\n"
        "      [minValue maxValue] : (Optional) Range for generating random numbers (default: 0 and 1000)\n"
        "      <outputFile>    : File where benchmark results (metrics) will be saved\n\n"
        "      <individualOutputFlag>   : --t for creating separate folder with .txt files of each repetition sorted data, --f for not creating such folder\n\n"
        "  STUDY1 MODE (VARY SIZES):\n"
        "    ./ProjectPath --study1 <algorithmFlag> <typeFlag> <repeatCount> <individualOutputFlag> <outputFile>\n"
        "      Runs five benchmarks with sizes {10000,20000,40000,80000,160000} and with random data input order\n"
        "      <repeatCount>       : Number of repetitions per size\n"
        "      <individualOutputFlag> : --t to save each run’s sorted data files, --f to skip\n\n"
        "  STUDY2 MODE (VARY DISTRIBUTIONS):\n"
        "    ./ProjectPath --study2 <algorithmFlag> <typeFlag> <size> <repeatCount> <individualOutputFlag> <outputFile>\n"
        "      Runs benchmarks on same size but different input orderings:\n"
        "      random, ascending, descending, 33% sorted, 66% sorted\n\n"
        "  STUDY3 MODE (VARY TYPES):\n"
        "    ./ProjectPath --study3 <algorithmFlag> <repeatCount> <individualOutputFlag> <outputFile>\n"
        "      Runs benchmarks on int, float, and double for the same sizes as in study 1 and random data input order\n\n"
        "  HELP MODE:\n"
        "    ./ProjectPath --help\n"
        "      Displays this help message\n\n"
        "Notes:\n"
        "  - If no arguments are provided, the help message is shown.\n"
        "  - The --file and --test modes are mutually exclusive.\n"
        << std::endl;
    }
};

#endif
