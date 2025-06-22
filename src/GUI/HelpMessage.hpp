#ifndef HELPMESSAGE_H
#define HELPMESSAGE_H

#include <iostream>

class HelpMessage {
public:
    static void printHelp() {
        std::cout <<
        "Usage:\n"
        "SORTING ALGORITHMS:\n"
        "       FILE INPUT MODE:\n"
        "           ./ProjectPath --file <algorithmFlag> <typeFlag> <inputFile> <outputFile>\n"
        "               <algorithmFlag> : --is for Insertion Sort, --bi for Binary Insertion Sort, --hs for Heap Sort, --qs for Quick Sort\n"
        "               <typeFlag>      : --i for int, --f for float, --d for double\n"
        "               <inputFile>     : Input file containing the data to be sorted\n"
        "               <outputFile>    : Output file for the sorted data\n\n"
        "       BENCHMARK MODE:\n"
        "           ./ProjectPath --benchmark <algorithmFlag> <typeFlag> <size> [minValue maxValue] <outputFile>\n"
        "               <algorithmFlag> : --is for Insertion Sort, --bi for Binary Insertion Sort, --hs for Heap Sort, --qs for Quick Sort\n"
        "               <typeFlag>      : --i for int, --f for float, --d for double\n"
        "               <size>          : Number of elements to generate\n"
        "               [minValue maxValue] : (Optional) Range for generating random numbers (default: 0 and 1000)\n"
        "               <outputFile>    : File where benchmark result (sorted output) will be saved\n\n"
        "       SERIES MODE:\n"
        "           ./ProjectPath --series <algorithmFlag> <typeFlag> <size> <repetitionCount> [minValue maxValue] <outputFile> <individualOutputFlag>\n"
        "               <algorithmFlag>     : --is for Insertion Sort, --bi for Binary Insertion Sort, --hs for Heap Sort, --qs for Quick Sort\n"
        "               <typeFlag>          : --i for int, --f for float, --d for double\n"
        "               <size>       : Number of elements to generate for each test run\n"
        "               <repetitionCount>  : Number of test repetitions to run the benchmark\n"
        "               [minValue maxValue] : (Optional) Range for generating random numbers (default: 0 and 1000)\n"
        "               <outputFile>    : File where benchmark results (metrics) will be saved\n\n"
        "               <individualOutputFlag>   : --t for creating separate folder with .txt files of each repetition sorted data, --f for not creating such folder\n\n"
        "       STUDY1 MODE (VARY SIZES):\n"
        "           ./ProjectPath --study1 <algorithmFlag> <typeFlag> <repeatCount> <individualOutputFlag> <outputFile>\n"
        "               Runs five benchmarks with sizes {10000,20000,40000,80000,160000} and with random data input order\n"
        "               <repeatCount>       : Number of repetitions per size\n"
        "               <individualOutputFlag> : --t to save each run’s sorted data files, --f to skip\n\n"
        "       STUDY2 MODE (VARY DISTRIBUTIONS):\n"
        "           ./ProjectPath --study2 <algorithmFlag> <typeFlag> <size> <repeatCount> <individualOutputFlag> <outputFile>\n"
        "               Runs benchmarks on same size but different input orderings:\n"
        "               random, ascending, descending, 33% sorted, 66% sorted\n\n"
        "       STUDY3 MODE (VARY TYPES):\n"
        "           ./ProjectPath --study3 <algorithmFlag> <repeatCount> <individualOutputFlag> <outputFile>\n"
        "               Runs benchmarks on int, float, and double for the same sizes as in study 1 and random data input order\n\n"
        "GRAPH ALGORITHMS:\n"
        "       FILE INPUT MODE:\n"
        "           ./ProjectPath --gFile <problemFlag> <algorithmFlag> <representationFlag> <inputFile> [outputFile] [sourceVertice] [destinationVertice]\n"
        "               <problemFlag>           : --mst for Minimum Spawning Tree, --tsp for Travelling Salesman Problem\n"
        "               <algorithmFlag>         : --prim for Prim Algorithm, --krus for Kruskal Algorithm, --dij for Dijkstri Algorithm, --ford for Ford-Bellman Algorithm, --all for All Algorithms of current problem\n"
        "               <representationFlag>    : --list for List of Neighbors as Graph representation, --matrix for Incydens Matrix as Graph representation, --all for All Accesible representations\n"
        "               <inputFile>             : Input file containing the graph. First row must contain number of Edges and Vertices\n"
        "               <outputFile>            : Output file for solution and timing\n"
        "               [sourceVertice]         : Edge where TSP problem will start from  \n"
        "               [destinationVertice]    : Edge where TSP problem will end at\n\n"
        "       BENCHMARK MODE:\n"
        "           ./ProjectPath --gBenchmark <problemFlag> <algorithmFlag> <representationFlag> <verticesCount> <density> <repetitionCount> <outputFile>\n"
        "               <problemFlag>           : --mst for Minimum Spawning Tree, --tsp for Travelling Salesman Problem\n"
        "               <algorithmFlag>         : --prim for Prim Algorithm, --krus for Kruskal Algorithm, --dij for Dijkstri Algorithm, --ford for Ford-Bellman Algorithm, --all for All Algorithms of current problem\n"
        "               <representationFlag>    : --list for List of Neighbors as Graph representation, --matrix for Incydens Matrix as Graph representation, --all for All Accesible representations\n"
        "               <verticesCount>         : Number of Vertices (Nodes)\n"
        "               <density>               : Density of edges \n"
        "               <repetitionCount>       : How many times test should be repeated (with graph regen)\n"
        "               <outputFile>            : Output file for solution and timing\n\n"
        "HELP MODE:\n"
        "  ./ProjectPath --help\n"
        "    Displays this help message\n\n"
        << std::endl;
    }
};

#endif
