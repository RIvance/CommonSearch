/**
 * @brief: This is a solution for a codeforces problem: https://codeforces.com/contest/1661/problem/B
 * @author: spanning-tree
 * @date: 2022-4-10
 */

#ifndef SEARCH_GETTINGZERO_H
#define SEARCH_GETTINGZERO_H

#include "Search.hpp"

namespace GettingZero {
    using namespace Search;

    using State = int;
    using Cost = int;
    using Action = int;

    class GettingZero : public Problem<State, Cost, Action> {
    private:
        const int mod = 32768;
    protected:
        bool isGoal(const State &state) const override {
            return false;
        }

        Vec<Action> getActions(const State &state) const override {
            if (state % 2 == 1) {
                return {0};
            }
            return {0, 1, 2};
        }

        State transferState(const State &state, const Action &action) const override {
            switch (action) {
                case 0:
                    return (state + mod - 1) % mod;
                case 1:
                    return state / 2;
                case 2:
                    return (state + mod) / 2;
                default:
                    return state;
            }
        }

    public:
        uint getDepth(const State &state) const {
            return nodeMap.find(state)->second.depth;
        }
    };

}
#endif //SEARCH_GETTINGZERO_H
