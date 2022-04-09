/**
 * @brief: Here are some simple tests for debugging
 * @author: spanning-tree
 * @date: 2022-4-10
 */

#include "Search.hpp"
#include "EightPuzzle.h"
#include "GettingZero.h"

void eightPuzzleTest() {
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
}

void gettingZeroTest() {
    int n;
    GettingZero::GettingZero gettingZero{};
    gettingZero.breadthFirstSearch();
    std::cin >> n;
    for(int i = 0; i < n; i++) {
        int a;
        std::cin >> a;
        std::cout << gettingZero.getDepth(a) << " ";
    }
    std::cout << std::endl;
}

int main() {
    eightPuzzleTest();
    gettingZeroTest();
    return 0;
}
