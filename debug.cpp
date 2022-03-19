//
// Created by infinite on 19/03/2022.
//
#include "Search.hpp"

using namespace Search;

using State = Vec<int>;
using Action = int;
using Cost = int;

class EightPuzzle : public Problem<State, Action, Cost, HashCombine<int>> {
protected:
    State targetState;
protected:
    bool isGoal(const Vec<int> &state) const override {
        return state == targetState;
    }

    Vec<Action> getActions(const State &state) const override {
        Vec<Vec<int>> actions{
                {1, 3},
                {0, 2, 4},
                {1, 5},
                {0, 4, 6},
                {1, 3, 5, 7},
                {2, 4, 8},
                {3, 7},
                {4, 6, 8},
                {7, 5}
        };
        for (uint i = 0; i < state.size(); i++) {
            if (state[i] == 0) {
                return actions[i];
            }
        }
        return {};
    }

    State transferState(const State &state, const Action &action) const override {
        State newState = state;
        for (uint i = 0; i < newState.size(); i++) {
            if (newState[i] == 0) {
                newState[i] = newState[action];
                newState[action] = 0;
                break;
            }
        }
        return newState;
    }

    static void printState(const State &state) {
        for (uint i = 0; i < 3; i++) {
            for (uint j = 0; j < 3; j++) {
                std::cout << state[i * 3 + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }


public:
    EightPuzzle(const State &start, State target) : targetState(std::move(target)) {
        setStartState(start);
    }

    void printPath() const override {
        if (!goalState) {
            std::cout << "Cannot access goal state" << std::endl;
            return;
        }
        std::cout << "Path: " << std::endl;
        State state = *goalState;
        do {
            printState(state);
            State parent = nodeMap.at(state).parent;
            if (parent == state) {
                break;
            }
            state = parent;
        } while (true);
    }

};

int main() {
    std::cout << "Hello World!" << std::endl;
    EightPuzzle problem{
            {1, 2, 3, 4, 5, 6, 7, 8, 0},
            {0, 1, 2, 3, 4, 5, 6, 7, 8}
    };
//    problem.breadthFirstSearch();
//    problem.printPath();
//    problem.depthFirstSearch();
//    problem.printPath();
//    problem.uniformCostSearch();
//    problem.printPath();
    problem.aStarSearch();
    problem.printPath();
    return 0;
}
