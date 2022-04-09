//
// Created by infinite on 19/03/2022.
//

#ifndef SEARCH_EIGHTPUZZLE_H
#define SEARCH_EIGHTPUZZLE_H
namespace EightPuzzle {
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

        Cost h(const State &state) const override {
            Cost cost = 0;
            for (uint i = 0; i < state.size(); i++) {
                cost += state[i] != targetState[i] ? 1 : 0;
            }
            return cost;
        }

        Cost g(const State &state, const Action &action) const override {
            return nodeMap.at(state).g + 1;
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
            int count = 0;
            do {
                count++;
                printState(state);
                State parent = nodeMap.at(state).parent;
                if (parent == state) {
                    break;
                }
                state = parent;
            } while (true);
            std::cout << "Total steps: " << count << std::endl;
        }

    };
}


#endif //SEARCH_EIGHTPUZZLE_H
