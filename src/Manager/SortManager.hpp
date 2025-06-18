#ifndef SORTMANAGER_H
#define SORTMANAGER_H

#include "../VektorImpl/Vektor.hpp"
#include "../RandomGenerator/RandomNumbersToSortGenerator.hpp"
#include "../SortAlgorithms/SortAlgorithms.hpp"
#include "../SortAlgorithms/AlgorithmsUtils.hpp"
#include "../FileIO/FileIO.hpp"
#include "TimerManager.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <filesystem>

class SortManager {
public:
    /// Reads data from an input file, sorts it with the given algorithm, and writes the result to an output file.
    template <typename T>
    static void fileInputMode(const char* algFlag, const char* inputFile, const char* outputFile) {
        int size = 0;
        Vektor<T> data = FileIO<T>::readData(inputFile, size);
        if (size == 0) {
            std::cerr << "No data read. Exiting." << std::endl;
            return;
        }
        std::cout << "Chosen algorithm: " << getAlgorithmName(algFlag) << std::endl;
        std::cout << "Chosen data type: " << getDataTypeName<T>() << std::endl;
        std::cout << "Data read from file with" << size << " elements." << std::endl;

        const int sortingTime = TimerManager::measureTimeMs( [&]() {
            sortUsingAlgorithm<T>(data, algFlag);
        });
        const bool isSorted = AlgorithmsUtils<T>::isSorted(data);

        std::cout << "Sorted = " << (isSorted ? "Yes" : "No")
                  << ", Time = " << sortingTime << " milliseconds." << std::endl;
        FileIO<T>::writeSortedData(data, outputFile);
    }

    /// Generates random data of specified size and range, sorts it, and outputs the result.
    template <typename T>
    static void benchmarkMode(const char* algFlag, int data_length, T minVal, T maxVal, const char* outputFile) {
        Vektor<T> data = RandomGenerator::generateDataToSort<T>(data_length, minVal, maxVal);

        std::cout << "Chosen algorithm: " << getAlgorithmName(algFlag) << std::endl;
        std::cout << "Chosen data type: " << getDataTypeName<T>() << std::endl;
        std::cout << "Data generated with " << data_length << " elements." << std::endl;

        const int sortingTime = TimerManager::measureTimeMs( [&]() {
            sortUsingAlgorithm<T>(data, algFlag);
        });
        const bool isSorted = AlgorithmsUtils<T>::isSorted(data);

        std::cout << "Sorted = " << (isSorted ? "Yes" : "No")
                  << ", Time = " << sortingTime << " milliseconds." << std::endl;
        FileIO<T>::writeSortedData(data, outputFile);
    }

    /**
    * Repeatedly generates or selects data according to mode, sorts it, records timings,
    * and optionally saves sorted data, history, and summary files.
    */
    template <typename T>
    static void serialBenchmark(const char* algFlag, int data_length, T minVal, T maxVal, int repeatCount,
        const char* outputFile, const char* writeFlag, const char* dataModeFlag = "--rand") {

        bool writeSorted = (std::strcmp(writeFlag, "--t") == 0);

        auto now= std::chrono::system_clock::now();
        auto in_time= std::chrono::system_clock::to_time_t(now);
        std::ostringstream ts;
        ts << std::put_time(std::localtime(&in_time), "%Y.%m.%d_%H-%M-%S");
        std::string timestamp = ts.str();

        std::string histName = std::string("benchmark_series_history_") + getAlgorithmName(algFlag) + ".txt";
        std::string summaryName   = std::string("benchmark_summary_history_") + getAlgorithmName(algFlag) + ".txt";

        std::string sortedFolder;
        if (writeSorted) {
            sortedFolder = "sorted_runs_" + timestamp;
            std::filesystem::create_directory(sortedFolder);
        }

        Vektor<int> times;
        for (int i = 0; i < repeatCount; ++i) {
            Vektor<T> data = selectDataByMode<T>(dataModeFlag, data_length, minVal, maxVal);
            int t = TimerManager::measureTimeMs([&](){
                SortManager::sortUsingAlgorithm<T>(data, algFlag);
            });
            const bool isSorted = AlgorithmsUtils<T>::isSorted(data);

            std::cout << "Sorted = " << (isSorted ? "Yes" : "No")
                      << ", Time = " << t << " milliseconds." << std::endl;
            times.push_back(t);
            if (writeSorted) {
                std::string sortedDataFileName = sortedFolder + "/" + std::to_string(i) + ".txt";
                FileIO<T>::writeSortedData(data, sortedDataFileName);
            }
            FileIO<T>::appendHistoryEntry(
                histName,
                timestamp,
                getSortingOrderName(dataModeFlag),
                i + 1,
                data_length,
                getAlgorithmName(algFlag),
                getDataTypeName<T>(),
                times[i]
            );
        }

        int minTime = times[0];
        int maxTime = times[0];
        long long sum = 0;
        for (int i = 0; i < times.size(); ++i) {
            int v = times[i];
            sum += v;
            if (v < minTime) minTime = v;
            if (v > maxTime) maxTime = v;
        }
        double average = double(sum) / repeatCount;

        SortAlgorithms<int>::quickSort(times);
        double median = (repeatCount % 2 == 0)
                      ? (times[repeatCount/2 - 1] + times[repeatCount/2]) / 2.0
                      : times[repeatCount/2];


        FileIO<T>::appendSummaryEntry(
         summaryName,
         timestamp,
         data_length,
         getSortingOrderName(dataModeFlag),
         getAlgorithmName(algFlag),
         getDataTypeName<T>(),
         repeatCount,
         minTime,
         maxTime,
         average,
         median,
         minVal,
         maxVal
    );


        std::cout << "Summary written to " << summaryName << "\n"
                  << "History appended to " << histName << std::endl;
        if (writeSorted)
            std::cout << "Sorted data files in folder: " << sortedFolder << std::endl;
    }

    /// Runs series of benchmarks varying the input size using the same algorithm and write options.
    template<typename T>
    static void studyVarySizes(const char* algFlag, int repeatCount, const char* writeFlag, const char* outputFile)
    {
        int sizes[] = {10000,20000,30000, 40000, 50000, 80000, 160000};
        T fullMin, fullMax;
        if constexpr (std::is_integral_v<T>) {
            fullMin = std::numeric_limits<T>::lowest();
            fullMax = std::numeric_limits<T>::max();
        } else {
            fullMin = T(-1000);
            fullMax = T(1000);
        }
        for (int sz : sizes) {
            serialBenchmark<T>(
                algFlag,
                sz,
                fullMin,
                fullMax,
                repeatCount,
                outputFile,
                writeFlag
            );
        }
    }

    /// Runs series of benchmarks varying the data distribution for a fixed size.
    template<typename T>
    static void studyVaryDistributions(const char* algFlag, int dataLength, int repeatCount, const char* writeFlag,
        const char* outputFile)
    {
        const char* distModes[] = {"--rand","--asc","--desc","--33","--66"};
        T fullMin = std::numeric_limits<T>::lowest();
        T fullMax = std::numeric_limits<T>::max();
        for (auto d : distModes) {
            std::cout << "[Study2] Mode = " << d << std::endl;
            try {
                serialBenchmark<T>(
                    algFlag,
                    dataLength,
                    fullMin,
                    fullMax,
                    repeatCount,
                    outputFile,
                    writeFlag,
                    d
                );
            }
            catch (const std::exception &e) {
                std::cerr << "Error for " << d << ": " << e.what() << std::endl;
            }
        }
    }

    /// Executes benchmarks across multiple data types (int, float, double).
    static void studyVaryTypes(const char* algFlag, int repeatCount, const char* writeFlag, const char* outputFile)
    {
        studyVarySizes<int>( algFlag, repeatCount, writeFlag, outputFile);
        studyVarySizes<float>( algFlag, repeatCount, writeFlag, outputFile);
        studyVarySizes<double>(algFlag, repeatCount, writeFlag, outputFile);
    }

private:
    /// Selects and generates data based on the given mode flag (random, sorted, etc.).
    template<typename T>
    static Vektor<T> selectDataByMode(const char* dataModeFlag, int dataLemgth, T minimum, T maximum) {
        if (std::strcmp(dataModeFlag, "--asc") == 0)
            return RandomGenerator::generateSortedData<T>(dataLemgth,  minimum, maximum);
        if (std::strcmp(dataModeFlag, "--desc") == 0)
            return RandomGenerator::generateReverseSortedData<T>(dataLemgth,  minimum, maximum);
        if (std::strcmp(dataModeFlag, "--33") == 0)
            return RandomGenerator::generate33PercentSortedData<T>(dataLemgth,  minimum, maximum);
        if (std::strcmp(dataModeFlag, "--66") == 0)
            return RandomGenerator::generate66PercentSortedData<T>(dataLemgth,  minimum, maximum);
        return RandomGenerator::generateDataToSort<T>(dataLemgth,  minimum, maximum);
    }

    /// Returns a human-readable name for the given sort order flag.
    static const char* getSortingOrderName(const char* orderFlag) {
        if (std::strcmp(orderFlag, "--rand") == 0)
            return "Random Order";
        if (std::strcmp(orderFlag, "--asc") == 0)
            return "Ascending Order";
        if (std::strcmp(orderFlag, "--desc") == 0)
            return "Descending Order";
        if (std::strcmp(orderFlag, "--33") == 0)
            return "33% Sorted";
        if (std::strcmp(orderFlag, "--66") == 0)
            return "66% Sorted";

        return "Unknown Sort Order";
    }

    /// Maps algorithm flags to their descriptive names.
    static const char* getAlgorithmName(const char* algFlag) {
        if (std::strcmp(algFlag, "--is") == 0)
            return "Insertion Sort";
        if (std::strcmp(algFlag, "--bi") == 0)
            return "Binary Insertion Sort";
        if (std::strcmp(algFlag, "--hs") == 0)
            return "Heap Sort";
        if (std::strcmp(algFlag, "--qs") == 0)
            return "Quick Sort";
        return "Unknown Algorithm";
    }

    /// Returns a textual representation of the template data type.
    template <typename T>
    static const char* getDataTypeName() {
        if constexpr (std::is_same_v<T, int>)
            return "Integer";
        if constexpr (std::is_same_v<T, float>)
            return "Float";
        if constexpr (std::is_same_v<T, double>)
            return "Double";
        else
            return "Unknown Data Type";
    }

    /// Dispatches to the appropriate sorting algorithm implementation based on the flag.
    template <typename T>
    static void sortUsingAlgorithm(Vektor<T>& data, const char* algFlag) {
        if (std::strcmp(algFlag, "--is") == 0)
            SortAlgorithms<T>::insertionSort(data);
        else if (std::strcmp(algFlag, "--bi") == 0)
            SortAlgorithms<T>::binaryInsertionSort(data);
        else if (std::strcmp(algFlag, "--hs") == 0)
            SortAlgorithms<T>::heapSort(data);
        else if (std::strcmp(algFlag, "--qs") == 0)
            SortAlgorithms<T>::quickSort(data);
        else {
            std::cerr << "Error: Invalid algorithm flag: " << algFlag << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
};

#endif
