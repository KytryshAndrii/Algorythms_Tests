#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <string>
#include <fstream>
#include <iostream>
#include "../VektorImpl/Vektor.hpp"

template <typename T>
class FileIO {
public:
    static Vektor<T> readData(const std::string &filename, int &size) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Error: Cannot open input file " << filename << std::endl;
            size = 0;
            return Vektor<T>();
        }
        infile >> size;
        Vektor<T> vector;
        T temp;
        for (int i = 0; i < size; i++) {
            infile >> temp;
            vector.push_back(temp);
        }
        infile.close();
        return vector;
    }

    static void writeSortedData(const Vektor<T>& data, const std::string &filename) {
        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Error: Cannot open output file " << filename << std::endl;
            return;
        }
        const int size = data.size();
        outfile << size << std::endl;
        for (int i = 0; i < size; i++) {
            outfile << data[i] << std::endl;
        }
        outfile.close();
    }

    static void appendHistoryEntry(const std::string &filename, const std::string &timeStamp, const char* sortingOrder,
        int repetitionNumber, int dataLength,  const char *algorithmFlag, const char *dataTypeFlag, int timeOfSorting) {
        std::ofstream ofs(filename, std::ios::app);
        if (!ofs) {
            std::cerr << "Error: Cannot open history file " << filename << std::endl;
            return;
        }
        ofs << timeStamp << ';'
            << repetitionNumber << ';'
            << dataLength << ";"
            << sortingOrder << ";"
            << algorithmFlag << ';'
            << dataTypeFlag << ';'
            << timeOfSorting << "\n";
        ofs.close();
    }

    static void appendSummaryEntry(const std::string &summaryHistoryFile, const std::string &timeStamp, int dataLength,
        const char *sortingOrder, const char *algorithmName, const char *dataTypeName, int repeatCount, int minTime,
        int maxTime, double average, double median, T minVal, T maxVal)
    {
        std::ofstream ofs(summaryHistoryFile, std::ios::app);
        if (!ofs) {
            std::cerr << "Error: Cannot open summary‐history file "
                      << summaryHistoryFile << std::endl;
            return;
        }
        // timestamp;numberOfElements;SortOrder;SortingAlgorithm;Type;Reps;Min;Max;Avg;Med;Range
        ofs << timeStamp << ';'
            << dataLength << ';'
            << sortingOrder << ';'
            << algorithmName << ';'
            << dataTypeName << ';'
            << repeatCount << ';'
            << minTime << ';'
            << maxTime << ';'
            << average << ';'
            << median << ';'
            << '[' << minVal << ',' << maxVal << ']'
            << "\n";
    }
};

#endif
