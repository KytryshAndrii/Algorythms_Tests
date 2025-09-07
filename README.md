# Sorting and Graph Algorithms – Benchmarking and Analysis

## Project Overview
This project implements and benchmarks **classical sorting algorithms** and **graph algorithms** in order to analyze their performance under conditions approximating real-world use cases.  

The work was conducted as part of an academic study with the goal of comparing **theoretical computational complexities** with **empirical results**, supported by automated experiment frameworks and visualization of results.  

The project is divided into two major parts:

### Sorting Algorithms
- Insertion Sort  
- Binary Insertion Sort  
- Heapsort  
- Quicksort (with randomized pivot)  

Benchmarks include:
- Varying input sizes (10k–160k elements)  
- Different data distributions (random, ascending, descending, partially sorted)  
- Different data types (`int`, `float`, `double`)  

### Graph Algorithms
- Minimum Spanning Tree (MST): Prim (list & matrix), Kruskal (list & matrix)  
- Shortest Path: Dijkstra (list & matrix), Bellman–Ford (list & matrix)  

Benchmarks include:
- Varying graph sizes (25–1000 vertices)  
- Different edge densities (25%, 50%, 99%)  
- Comparison of list vs. matrix representations  

---

## Key Findings

### Sorting
- Insertion Sort and Binary Insertion Sort scale quadratically and become impractical beyond ~80k elements.  
- Quicksort consistently achieves the fastest execution times (7–9 ms for 160k elements) but is sensitive to pivot choice.  
- Heapsort is slower (12–15 ms) but guarantees **O(n log n)** regardless of input.  
- Data type has only marginal impact; floating-point introduces minor overhead.  

### Graphs
- Kruskal (**O(E log E)**) and Dijkstra with priority queue (**O((V+E) log V)**) outperform Prim and Bellman–Ford in practical scenarios.  
- Dense matrix-based implementations degrade significantly, while list-based ones remain efficient.  
- Results are consistent with theoretical expectations, with 50+ experiments per configuration (1350 graphs in total).  

---
## Usage Help

You can display the built-in help message by running:

```bash
./ProjectPath --help

Usage:
SORTING ALGORITHMS:
       FILE INPUT MODE:
           ./ProjectPath --file <algorithmFlag> <typeFlag> <inputFile> <outputFile>
               <algorithmFlag> : --is for Insertion Sort, --bi for Binary Insertion Sort, --hs for Heap Sort, --qs for Quick Sort
               <typeFlag>      : --i for int, --f for float, --d for double
               <inputFile>     : Input file containing the data to be sorted
               <outputFile>    : Output file for the sorted data

       BENCHMARK MODE:
           ./ProjectPath --benchmark <algorithmFlag> <typeFlag> <size> [minValue maxValue] <outputFile>
               <algorithmFlag> : --is for Insertion Sort, --bi for Binary Insertion Sort, --hs for Heap Sort, --qs for Quick Sort
               <typeFlag>      : --i for int, --f for float, --d for double
               <size>          : Number of elements to generate
               [minValue maxValue] : (Optional) Range for generating random numbers (default: 0 and 1000)
               <outputFile>    : File where benchmark result (sorted output) will be saved

       SERIES MODE:
           ./ProjectPath --series <algorithmFlag> <typeFlag> <size> <repetitionCount> [minValue maxValue] <outputFile> <individualOutputFlag>
               <algorithmFlag>     : --is for Insertion Sort, --bi for Binary Insertion Sort, --hs for Heap Sort, --qs for Quick Sort
               <typeFlag>          : --i for int, --f for float, --d for double
               <size>              : Number of elements to generate for each test run
               <repetitionCount>   : Number of test repetitions to run the benchmark
               [minValue maxValue] : (Optional) Range for generating random numbers (default: 0 and 1000)
               <outputFile>        : File where benchmark results (metrics) will be saved
               <individualOutputFlag>   : --t for creating separate folder with .txt files of each repetition sorted data, --f for not creating such folder

       STUDY1 MODE (VARY SIZES):
           ./ProjectPath --study1 <algorithmFlag> <typeFlag> <repeatCount> <individualOutputFlag> <outputFile>
               Runs five benchmarks with sizes {10000,20000,40000,80000,160000} and with random data input order
               <repeatCount>       : Number of repetitions per size
               <individualOutputFlag> : --t to save each run’s sorted data files, --f to skip

       STUDY2 MODE (VARY DISTRIBUTIONS):
           ./ProjectPath --study2 <algorithmFlag> <typeFlag> <size> <repeatCount> <individualOutputFlag> <outputFile>
               Runs benchmarks on same size but different input orderings:
               random, ascending, descending, 33% sorted, 66% sorted

       STUDY3 MODE (VARY TYPES):
           ./ProjectPath --study3 <algorithmFlag> <repeatCount> <individualOutputFlag> <outputFile>
               Runs benchmarks on int, float, and double for the same sizes as in study 1 and random data input order

GRAPH ALGORITHMS:
       FILE INPUT MODE:
           ./ProjectPath --gFile <problemFlag> <algorithmFlag> <representationFlag> <inputFile> [outputFile] [sourceVertice] [destinationVertice]
               <problemFlag>           : --mst for Minimum Spanning Tree, --tsp for Travelling Salesman Problem
               <algorithmFlag>         : --prim for Prim Algorithm, --krus for Kruskal Algorithm, --dij for Dijkstra Algorithm, --ford for Ford-Bellman Algorithm, --all for All Algorithms of current problem
               <representationFlag>    : --list for Adjacency List as Graph representation, --matrix for Incidence Matrix as Graph representation, --all for All Accessible representations
               <inputFile>             : Input file containing the graph. First row must contain number of Edges and Vertices
               <outputFile>            : Output file for solution and timing
               [sourceVertice]         : Vertex where TSP problem will start from
               [destinationVertice]    : Vertex where TSP problem will end at

       BENCHMARK MODE:
           ./ProjectPath --gBenchmark <problemFlag> <algorithmFlag> <representationFlag> <verticesCount> <density> <repetitionCount> <outputFile>
               <problemFlag>           : --mst for Minimum Spanning Tree, --tsp for Travelling Salesman Problem
               <algorithmFlag>         : --prim for Prim Algorithm, --krus for Kruskal Algorithm, --dij for Dijkstra Algorithm, --ford for Ford-Bellman Algorithm, --all for All Algorithms of current problem
               <representationFlag>    : --list for Adjacency List as Graph representation, --matrix for Incidence Matrix as Graph representation, --all for All Accessible representations
               <verticesCount>         : Number of Vertices (Nodes)
               <density>               : Density of edges
               <repetitionCount>       : How many times test should be repeated (with graph regen)
               <outputFile>            : Output file for solution and timing

HELP MODE:
  ./ProjectPath --help
    Displays this help message

```

## Examples of usage
```
# Sort integers from file "input.txt" into "output.txt" using Quick Sort
./ProjectPath --file --qs --i input.txt output.txt

# Run a benchmark with 50,000 random floats using Heap Sort
./ProjectPath --benchmark --hs --f 50000 0 1000 heap_results.txt

# Study1: Run 5 different input sizes for Binary Insertion Sort on doubles
./ProjectPath --study1 --bi --d 10 --f study1_results.txt


# Solve MST using Kruskal on adjacency list from input graph file
./ProjectPath --gFile --mst --krus --list graph.txt mst_output.txt

# Run Dijkstra shortest path benchmark on 200 vertices, density 50%, repeated 10 times
./ProjectPath --gBenchmark --tsp --dij --list 200 50 10 tsp_benchmark.txt

# Solve MST with all algorithms and both representations
./ProjectPath --gFile --mst --all --all graph.txt mst_comparison.txt

```
## Building the Project
This project is written in **C++17** and requires a standard compiler such as `g++` or `clang++`.  
```bash
mkdir build && cd build
cmake ..
make
```
