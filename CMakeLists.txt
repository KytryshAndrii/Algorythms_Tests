cmake_minimum_required(VERSION 3.30)
project(AIZO_C++)

set(CMAKE_CXX_STANDARD 20)

add_executable(AIZO_C++
#        main.cpp
        test.cpp
        src/Timer/Timer.cpp
        src/Timer/Timer.hpp
        src/FileIO/FileIO.hpp
        src/VektorImpl/Vektor.hpp
        src/RandomGenerator/RandomNumbersToSortGenerator.hpp
        src/SortAlgorithms/SortAlgorithms.hpp
        src/SortAlgorithms/AlgorithmsUtils.hpp
        src/GUI/HelpMessage.hpp
        src/Manager/SortManager.hpp
        src/Manager/TimerManager.hpp
        src/GraphsImpl/GraphMatrix.hpp
        src/GraphsImpl/GraphList.hpp
        src/FileIO/GraphFileIO.hpp
        src/common/GraphRepresentingShape.hpp
        src/GraphsImpl/IGraph.hpp
        src/RandomGenerator/RandomGraphGenerator.hpp
        src/common/GraphFactory.hpp
        src/common/EdgeTripple.hpp
        src/GraphsAlgorithms/MSTProblem/KruskalList.hpp
        src/GraphsAlgorithms/MSTProblem/KruskalMatrix.hpp
        src/GraphsAlgorithms/MSTProblem/PrimList.hpp
        src/GraphsAlgorithms/MSTProblem/PrimMatrix.hpp
        src/common/UnionJoinSet.hpp
        src/GraphsAlgorithms/TSPProblem/DijkstraList.hpp
        src/GraphsAlgorithms/TSPProblem/DijkstraMatrix.hpp
        src/HeapImpl/MinHeap.hpp
        src/GraphsAlgorithms/TSPProblem/FordBellmanMatrix.hpp
        src/GraphsAlgorithms/TSPProblem/FordBellmanList.hpp
        src/Manager/GraphsManager.hpp)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_options(AIZO_C++ PRIVATE
            -static
            -static-libgcc
            -static-libstdc++
            -pthread
    )
endif()