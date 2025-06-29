#include <iostream>
#include <cstring>
#include <cstdlib>
#include "./src/Manager/SortManager.hpp"
#include "./src/Manager/GraphsManager.hpp"
#include "./src/GUI/HelpMessage.hpp"

int main(int argc, char* argv[]) {
    std::cout<< std::unitbuf;
    if (argc < 2 || std::strcmp(argv[1], "--help") == 0) {
        HelpMessage::printHelp();
        return 0;
    }

    std::string mode = argv[1];

    if (mode == "--file") {
        if (argc < 6) {
            std::cerr << "[ERROR] Not enough arguments for --file mode." << std::endl;
            HelpMessage::printHelp();
            return 1;
        }
        const char* algFlag = argv[2];
        const char* typeFlag = argv[3];
        const char* inputFile = argv[4];
        const char* outputFile = argv[5];

        if (std::strcmp(typeFlag, "--i") == 0)
            SortManager::fileInputMode<int>(algFlag, inputFile, outputFile);
        else if (std::strcmp(typeFlag, "--f") == 0)
            SortManager::fileInputMode<float>(algFlag, inputFile, outputFile);
        else if (std::strcmp(typeFlag, "--d") == 0)
            SortManager::fileInputMode<double>(algFlag, inputFile, outputFile);
        else {
            std::cerr << "[ERROR] Invalid type flag: " << typeFlag << std::endl;
            HelpMessage::printHelp();
            return 1;
        }
    }
    else if (mode == "--benchmark") {
        if (argc < 6) {
            std::cerr << "[ERROR] Not enough arguments for --benchmark mode." << std::endl;
            HelpMessage::printHelp();
            return 1;
        }
        const char* algFlag = argv[2];
        const char* typeFlag = argv[3];
        int data_length = std::atoi(argv[4]);
        char* outputFile = argv[5];

        if ((std::strcmp(typeFlag, "--i") == 0 && argc >= 7) ||
            ((std::strcmp(typeFlag, "--f") == 0 || std::strcmp(typeFlag, "--d") == 0) && argc >= 7)) {
            outputFile = argv[7];
            if (argc < 8) {
                std::cerr << "[Error] Not enough arguments for --test mode with specified range." << std::endl;
                HelpMessage::printHelp();
                return 1;
            }
            if (std::strcmp(typeFlag, "--i") == 0) {
                int minVal = std::atoi(argv[5]);
                int maxVal = std::atoi(argv[6]);
                SortManager::benchmarkMode<int>(algFlag, data_length, minVal, maxVal, outputFile);
            }
            else if (std::strcmp(typeFlag, "--f") == 0) {
                float minVal = std::atof(argv[5]);
                float maxVal = std::atof(argv[6]);
                SortManager::benchmarkMode<float>(algFlag, data_length, minVal, maxVal, outputFile);
            }
            else if (std::strcmp(typeFlag, "--d") == 0) {
                double minVal = std::atof(argv[5]);
                double maxVal = std::atof(argv[6]);
                SortManager::benchmarkMode<double>(algFlag, data_length, minVal, maxVal, outputFile);
            }
        }
        else {
            if (std::strcmp(typeFlag, "--i") == 0) {
                SortManager::benchmarkMode<int>(algFlag, data_length, 0, 1000, outputFile);
            }
            else if (std::strcmp(typeFlag, "--f") == 0) {
                SortManager::benchmarkMode<float>(algFlag, data_length, 0.0f, 1000.0f, outputFile);
            }
            else if (std::strcmp(typeFlag, "--d") == 0) {
                SortManager::benchmarkMode<double>(algFlag, data_length, 0.0, 1000.0, outputFile);
            }
            else {
                std::cerr << "Error: Invalid type flag: " << typeFlag << std::endl;
                HelpMessage::printHelp();
                return 1;
            }
        }
    }
    else if (mode == "--series") {
        if (argc != 8 && argc != 10) {
            std::cerr << "[ERROR] Invalid number of arguments for --series mode." << std::endl;
            HelpMessage::printHelp();
            return 1;
        }

        const char* algFlag = argv[2];
        const char* typeFlag = argv[3];
        int data_length = std::atoi(argv[4]);
        int repetitions = 0;
        const char* outputFile = nullptr;
        const char* folderRunsOutputFlag = nullptr;

        if (argc == 8) {
            repetitions = std::atoi(argv[5]);
            outputFile = argv[6];
            folderRunsOutputFlag = argv[7];
        } else {
            if (std::strcmp(typeFlag, "--i") == 0) {
                int minVal = std::atoi(argv[6]);
                int maxVal = std::atoi(argv[7]);
                repetitions = std::atoi(argv[5]);
                outputFile = argv[8];
                folderRunsOutputFlag = argv[9];
                SortManager::serialBenchmark<int>(algFlag, data_length, minVal, maxVal, repetitions, outputFile, folderRunsOutputFlag);
                return 0;
            }  if (std::strcmp(typeFlag, "--f") == 0) {
                float minVal = std::atof(argv[6]);
                float maxVal = std::atof(argv[7]);
                repetitions = std::atoi(argv[5]);
                outputFile = argv[8];
                folderRunsOutputFlag = argv[9];
                SortManager::serialBenchmark<float>(algFlag, data_length, minVal, maxVal, repetitions, outputFile, folderRunsOutputFlag);
                return 0;
            } if (std::strcmp(typeFlag, "--d") == 0) {
                double minVal = std::atof(argv[6]);
                double maxVal = std::atof(argv[7]);
                repetitions = std::atoi(argv[5]);
                outputFile = argv[8];
                folderRunsOutputFlag = argv[9];
                SortManager::serialBenchmark<double>(algFlag, data_length, minVal, maxVal, repetitions, outputFile, folderRunsOutputFlag);
                return 0;
            }
        }
        if (std::strcmp(typeFlag, "--i") == 0) {
            SortManager::serialBenchmark<int>(algFlag, data_length, 0, 1000, repetitions, outputFile, folderRunsOutputFlag);
        }
        else if (std::strcmp(typeFlag, "--f") == 0) {
            SortManager::serialBenchmark<float>(algFlag, data_length, 0.0f, 1000.0f, repetitions, outputFile, folderRunsOutputFlag);
        }
        else if (std::strcmp(typeFlag, "--d") == 0) {
            SortManager::serialBenchmark<double>(algFlag, data_length, 0.0, 1000.0, repetitions, outputFile, folderRunsOutputFlag);
        }
        else {
            std::cerr << "[ERROR] Invalid type flag: " << typeFlag << std::endl;
            HelpMessage::printHelp();
            return 1;
        }
    }
    else if (mode == "--study1") {
        const char* algFlag     = argv[2];
        const char* typeFlag    = argv[3];
        int repeatCount = std::atoi(argv[4]);
        const char* writeFlag   = argv[5];
        const char* outputFile  = argv[6];

        if      (std::strcmp(typeFlag, "--i") == 0)
            SortManager::studyVarySizes<int>(algFlag, repeatCount, writeFlag, outputFile);
        else if (std::strcmp(typeFlag, "--f") == 0)
            SortManager::studyVarySizes<float>(algFlag, repeatCount, writeFlag, outputFile);
        else
            SortManager::studyVarySizes<double>(algFlag, repeatCount, writeFlag, outputFile);
    }
    else if (mode == "--study2") {
        const char* algFlag     = argv[2];
        const char* typeFlag    = argv[3];
        int dataLength  = std::atoi(argv[4]);
        int repeatCount = std::atoi(argv[5]);
        const char* writeFlag   = argv[6];
        const char* outputFile  = argv[7];

        if      (std::strcmp(typeFlag, "--i") == 0)
            SortManager::studyVaryDistributions<int>(algFlag, dataLength, repeatCount, writeFlag, outputFile);
        else if (std::strcmp(typeFlag, "--f") == 0)
            SortManager::studyVaryDistributions<float>(algFlag, dataLength, repeatCount, writeFlag, outputFile);
        else
            SortManager::studyVaryDistributions<double>(algFlag, dataLength, repeatCount, writeFlag, outputFile);
    }
    else if (mode == "--study3") {
        const char* algFlag     = argv[2];
        int repeatCount = std::atoi(argv[3]);
        const char* writeFlag   = argv[4];
        const char* outputFile  = argv[5];

        SortManager::studyVaryTypes(algFlag, repeatCount, writeFlag, outputFile);
    }
    else if (mode == "--gFile") {
        if (argc < 6) {
            std::cerr << "[ERROR] Not enough arguments for --gFile mode." << std::endl;
            HelpMessage::printHelp();
            return 1;
        }
        const char* problemFlag   = argv[2];
        const char* algorithmFlag = argv[3];
        const char* shapeFlag     = argv[4];
        const std::string inputFile     = argv[5];

        const int difference = argc - 6;
        const char* outputFile = "";
        int src = 0, dst = -1;

        if (std::strcmp(problemFlag, "--tsp") == 0) {
            // for TSP, you may supply:
            //   - nothing (rem==0)
            //   - just outputFile       (rem==1)
            //   - src and dst           (rem==2)
            //   - outputFile, src, dst  (rem==3)
            if (difference == 1) {
                outputFile = argv[6];
            }
            else if (difference == 2) {
                src = std::atoi(argv[6]);
                dst = std::atoi(argv[7]);
            }
            else if (difference == 3) {
                outputFile = argv[6];
                src        = std::atoi(argv[7]);
                dst        = std::atoi(argv[8]);
            }
            else if (difference != 0) {
                std::cerr << "[ERROR] Invalid number of arguments for --gFile with --tsp." << std::endl;
                HelpMessage::printHelp();
                return 1;
            }
            if ((difference == 2 || difference == 3) && dst < 0) {
                std::cerr << "[ERROR] Both source and destination vertices must be specified for --tsp." << std::endl;
                HelpMessage::printHelp();
                return 1;
            }
        }
        else {
            if (difference > 1) {
                std::cerr << "[ERROR] Too many arguments for --gFile with --mst." << std::endl;
                HelpMessage::printHelp();
                return 1;
            }
            if (difference == 1) {
                outputFile = argv[6];
            }
        }
        GraphsManager::fileInputMode(problemFlag, algorithmFlag, shapeFlag, inputFile, outputFile, src, dst);
    }
    else if (mode == "--gBenchmark") {
        if (argc < 8) {
            std::cerr << "[ERROR] Not enough arguments for --gBenchmark mode." << std::endl;
            HelpMessage::printHelp();
            return 1;
        }
        if (argc > 9) {
            std::cerr << "[ERROR] A lot of arguments for --gBenchmark mode." << std::endl;
            HelpMessage::printHelp();
            return 1;
        }
        const char* problemFlag   = argv[2];
        const char* algorithmFlag = argv[3];
        const char* shapeFlag     = argv[4];
        int verticesCount         = std::atoi(argv[5]);
        double density            = std::atof(argv[6]);
        int repeatCount4          = std::atoi(argv[7]);
        const char* outputFile4   = argv[8];
        GraphsManager::benchmarkMode(problemFlag, algorithmFlag, shapeFlag,
                                     verticesCount, density, repeatCount4, outputFile4);
    }
    else {
        std::cerr << "[ERROR] Unknown mode: " << mode << std::endl;
        HelpMessage::printHelp();
        return 1;
    }

    return 0;
}
