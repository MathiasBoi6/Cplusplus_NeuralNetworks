#pragma once
#include <vector>


template<typename T>
void printVector2D(const std::vector<std::vector<T>>& vec) {
    for (const auto& innerVec : vec) {
        for (const auto& element : innerVec) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}