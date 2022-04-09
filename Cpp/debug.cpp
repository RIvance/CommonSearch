//
// Created by infinite on 19/03/2022.
//
#include "Search.hpp"
#include "EightPuzzle.h"

int main() {
    std::cout << "Test for eight puzzle" << std::endl;
    EightPuzzle::EightPuzzle eightPuzzle{
            {1, 2, 3, 4, 5, 6, 7, 8, 0},
            {0, 1, 2, 3, 4, 5, 6, 7, 8}
    };
    eightPuzzle.breadthFirstSearch();
    eightPuzzle.printPath();
    eightPuzzle.depthFirstSearch();
    eightPuzzle.printPath();
    eightPuzzle.uniformCostSearch();
    eightPuzzle.printPath();
    eightPuzzle.greedySearch();
    eightPuzzle.printPath();
    eightPuzzle.aStarSearch();
    eightPuzzle.printPath();
    return 0;
}
