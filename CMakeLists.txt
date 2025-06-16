cmake_minimum_required(VERSION 3.30)
project(AIZO_C++)

set(CMAKE_CXX_STANDARD 20)

add_executable(AIZO_C++
        main.cpp
        src/Timer/Timer.cpp
        src/Timer/Timer.hpp
        src/FileIO/FileIO.hpp
        src/VektorImpl/Vektor.hpp
        src/RandomGenerator/RandomGenerator.hpp
        src/SortAlgorithms/SortAlgorithms.hpp
        src/SortAlgorithms/AlgorithmsUtils.hpp
        src/GUI/HelpMessage.hpp
        src/Manager/SortManager.hpp
        src/Manager/TimerManager.hpp
        src/Graphs/GraphMatrix.hpp
        src/Graphs/GraphList.hpp
        test.cpp
        src/FileIO/GraphFileIO.hpp
        src/common/GraphRepresentingShape.hpp
        src/Graphs/IGraph.hpp)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_options(AIZO_C++ PRIVATE
            -static
            -static-libgcc
            -static-libstdc++
            -pthread
    )
endif()