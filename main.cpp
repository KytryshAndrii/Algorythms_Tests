// #include <iostream>
// #include <cstring>
// #include <cstdlib>
// #include "./src/Manager/SortManager.hpp"
// #include "./src/GUI/HelpMessage.hpp"
//
// int main(int argc, char* argv[]) {
//     std::cout<< std::unitbuf;
//     if (argc < 2 || std::strcmp(argv[1], "--help") == 0) {
//         HelpMessage::printHelp();
//         return 0;
//     }
//
//     std::string mode = argv[1];
//
//     if (mode == "--file") {
//         if (argc < 6) {
//             std::cerr << "Error: Not enough arguments for --file mode." << std::endl;
//             HelpMessage::printHelp();
//             return 1;
//         }
//         const char* algFlag = argv[2];
//         const char* typeFlag = argv[3];
//         const char* inputFile = argv[4];
//         const char* outputFile = argv[5];
//
//         if (std::strcmp(typeFlag, "--i") == 0)
//             SortManager::fileInputMode<int>(algFlag, inputFile, outputFile);
//         else if (std::strcmp(typeFlag, "--f") == 0)
//             SortManager::fileInputMode<float>(algFlag, inputFile, outputFile);
//         else if (std::strcmp(typeFlag, "--d") == 0)
//             SortManager::fileInputMode<double>(algFlag, inputFile, outputFile);
//         else {
//             std::cerr << "Error: Invalid type flag: " << typeFlag << std::endl;
//             HelpMessage::printHelp();
//             return 1;
//         }
//     }
//     else if (mode == "--benchmark") {
//         if (argc < 6) {
//             std::cerr << "Error: Not enough arguments for --benchmark mode." << std::endl;
//             HelpMessage::printHelp();
//             return 1;
//         }
//         const char* algFlag = argv[2];
//         const char* typeFlag = argv[3];
//         int data_length = std::atoi(argv[4]);
//         char* outputFile = argv[5];
//
//         if ((std::strcmp(typeFlag, "--i") == 0 && argc >= 7) ||
//             ((std::strcmp(typeFlag, "--f") == 0 || std::strcmp(typeFlag, "--d") == 0) && argc >= 7)) {
//             outputFile = argv[7];
//             if (argc < 8) {
//                 std::cerr << "Error: Not enough arguments for --test mode with specified range." << std::endl;
//                 HelpMessage::printHelp();
//                 return 1;
//             }
//             if (std::strcmp(typeFlag, "--i") == 0) {
//                 int minVal = std::atoi(argv[5]);
//                 int maxVal = std::atoi(argv[6]);
//                 SortManager::benchmarkMode<int>(algFlag, data_length, minVal, maxVal, outputFile);
//             }
//             else if (std::strcmp(typeFlag, "--f") == 0) {
//                 float minVal = std::atof(argv[5]);
//                 float maxVal = std::atof(argv[6]);
//                 SortManager::benchmarkMode<float>(algFlag, data_length, minVal, maxVal, outputFile);
//             }
//             else if (std::strcmp(typeFlag, "--d") == 0) {
//                 double minVal = std::atof(argv[5]);
//                 double maxVal = std::atof(argv[6]);
//                 SortManager::benchmarkMode<double>(algFlag, data_length, minVal, maxVal, outputFile);
//             }
//         }
//         else {
//             if (std::strcmp(typeFlag, "--i") == 0) {
//                 SortManager::benchmarkMode<int>(algFlag, data_length, 0, 1000, outputFile);
//             }
//             else if (std::strcmp(typeFlag, "--f") == 0) {
//                 SortManager::benchmarkMode<float>(algFlag, data_length, 0.0f, 1000.0f, outputFile);
//             }
//             else if (std::strcmp(typeFlag, "--d") == 0) {
//                 SortManager::benchmarkMode<double>(algFlag, data_length, 0.0, 1000.0, outputFile);
//             }
//             else {
//                 std::cerr << "Error: Invalid type flag: " << typeFlag << std::endl;
//                 HelpMessage::printHelp();
//                 return 1;
//             }
//         }
//     }
//     else if (mode == "--series") {
//         if (argc != 8 && argc != 10) {
//             std::cerr << "Error: Invalid number of arguments for --series mode." << std::endl;
//             HelpMessage::printHelp();
//             return 1;
//         }
//
//         const char* algFlag = argv[2];
//         const char* typeFlag = argv[3];
//         int data_length = std::atoi(argv[4]);
//         int repetitions = 0;
//         const char* outputFile = nullptr;
//         const char* folderRunsOutputFlag = nullptr;
//
//         if (argc == 8) {
//             repetitions = std::atoi(argv[5]);
//             outputFile = argv[6];
//             folderRunsOutputFlag = argv[7];
//         } else {
//             if (std::strcmp(typeFlag, "--i") == 0) {
//                 int minVal = std::atoi(argv[6]);
//                 int maxVal = std::atoi(argv[7]);
//                 repetitions = std::atoi(argv[5]);
//                 outputFile = argv[8];
//                 folderRunsOutputFlag = argv[9];
//                 SortManager::serialBenchmark<int>(algFlag, data_length, minVal, maxVal, repetitions, outputFile, folderRunsOutputFlag);
//                 return 0;
//             }  if (std::strcmp(typeFlag, "--f") == 0) {
//                 float minVal = std::atof(argv[6]);
//                 float maxVal = std::atof(argv[7]);
//                 repetitions = std::atoi(argv[5]);
//                 outputFile = argv[8];
//                 folderRunsOutputFlag = argv[9];
//                 SortManager::serialBenchmark<float>(algFlag, data_length, minVal, maxVal, repetitions, outputFile, folderRunsOutputFlag);
//                 return 0;
//             } if (std::strcmp(typeFlag, "--d") == 0) {
//                 double minVal = std::atof(argv[6]);
//                 double maxVal = std::atof(argv[7]);
//                 repetitions = std::atoi(argv[5]);
//                 outputFile = argv[8];
//                 folderRunsOutputFlag = argv[9];
//                 SortManager::serialBenchmark<double>(algFlag, data_length, minVal, maxVal, repetitions, outputFile, folderRunsOutputFlag);
//                 return 0;
//             }
//         }
//         if (std::strcmp(typeFlag, "--i") == 0) {
//             SortManager::serialBenchmark<int>(algFlag, data_length, 0, 1000, repetitions, outputFile, folderRunsOutputFlag);
//         }
//         else if (std::strcmp(typeFlag, "--f") == 0) {
//             SortManager::serialBenchmark<float>(algFlag, data_length, 0.0f, 1000.0f, repetitions, outputFile, folderRunsOutputFlag);
//         }
//         else if (std::strcmp(typeFlag, "--d") == 0) {
//             SortManager::serialBenchmark<double>(algFlag, data_length, 0.0, 1000.0, repetitions, outputFile, folderRunsOutputFlag);
//         }
//         else {
//             std::cerr << "Error: Invalid type flag: " << typeFlag << std::endl;
//             HelpMessage::printHelp();
//             return 1;
//         }
//     }
//     else if (mode == "--study1") {
//         const char* algFlag     = argv[2];
//         const char* typeFlag    = argv[3];
//         int repeatCount = std::atoi(argv[4]);
//         const char* writeFlag   = argv[5];
//         const char* outputFile  = argv[6];
//
//         if      (std::strcmp(typeFlag, "--i") == 0)
//             SortManager::studyVarySizes<int>(algFlag, repeatCount, writeFlag, outputFile);
//         else if (std::strcmp(typeFlag, "--f") == 0)
//             SortManager::studyVarySizes<float>(algFlag, repeatCount, writeFlag, outputFile);
//         else
//             SortManager::studyVarySizes<double>(algFlag, repeatCount, writeFlag, outputFile);
//     }
//     else if (mode == "--study2") {
//         const char* algFlag     = argv[2];
//         const char* typeFlag    = argv[3];
//         int dataLength  = std::atoi(argv[4]);
//         int repeatCount = std::atoi(argv[5]);
//         const char* writeFlag   = argv[6];
//         const char* outputFile  = argv[7];
//
//         if      (std::strcmp(typeFlag, "--i") == 0)
//             SortManager::studyVaryDistributions<int>(algFlag, dataLength, repeatCount, writeFlag, outputFile);
//         else if (std::strcmp(typeFlag, "--f") == 0)
//             SortManager::studyVaryDistributions<float>(algFlag, dataLength, repeatCount, writeFlag, outputFile);
//         else
//             SortManager::studyVaryDistributions<double>(algFlag, dataLength, repeatCount, writeFlag, outputFile);
//     }
//     else if (mode == "--study3") {
//         const char* algFlag     = argv[2];
//         int repeatCount = std::atoi(argv[3]);
//         const char* writeFlag   = argv[4];
//         const char* outputFile  = argv[5];
//
//         SortManager::studyVaryTypes(algFlag, repeatCount, writeFlag, outputFile);
//     }
//     else {
//         std::cerr << "Error: Unknown mode: " << mode << std::endl;
//         HelpMessage::printHelp();
//         return 1;
//     }
//
//     return 0;
// }
