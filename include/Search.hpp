//
// Created by infinite on 19/03/2022.
//

#ifndef SEARCH_SEARCH_HPP
#define SEARCH_SEARCH_HPP

#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <iostream>

namespace Search {

    template<typename T>
    using Vec = std::vector<T>;

    template<typename T>
    using PriorityQueue = std::priority_queue<T>;

    template<typename T>
    using Queue = std::queue<T>;

    template<typename T>
    using Stack = std::stack<T>;

    template<typename T1, typename T2, class Hash>
    using HashMap = std::unordered_map<T1, T2, Hash>;

    template<typename T>
    using Option = std::optional<T>;

    using std::nullopt;

    template<typename T>
    class [[maybe_unused]] HashCombine {
    public:
        uint operator()(Vec<T> const &vec) const {
            std::size_t seed = vec.size();
            for (auto &i: vec) {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;;
        }
    };


    template<typename State, typename Action, typename Cost = int, class Hash = std::hash<State>>
    class Problem {
    protected:
        struct Node {
            State parent;
            State state;
            Cost cost;
            uint depth;

            bool operator<(const Node &node) const {
                return cost < node.cost;
            }
        };

        virtual bool isGoal(const State &state) const = 0;

        virtual Vec<Action> getActions(const State &state) const = 0;

        virtual Cost g(const State &state, const Action &action) const {
            return 0;
        };

        virtual Cost h(const State &state) const {
            return 0;
        };

        virtual State transferState(const State &state, const Action &action) const = 0;

        virtual bool isValid(const State &state) const {
            return true;
        }

        void clear() {
            goalState = nullopt;
            Stack<Node> empty1;
            Queue<Node> empty2;
            PriorityQueue<Node> empty3;
            HashMap<State, Node, Hash> empty4;
            std::swap(empty1, stack);
            std::swap(empty2, queue);
            std::swap(empty3, priorityQueue);
            std::swap(empty4, nodeMap);
        }

    protected:
        State startState;

        Option<State> goalState;

        Stack<Node> stack;

        Queue<Node> queue;

        PriorityQueue<Node> priorityQueue;

        HashMap<State, Node, Hash> nodeMap;


    public:
        void setStartState(const State &state) {
            startState = state;
        }

        Option<State> breadthFirstSearch() {
            clear();
            auto makeNode = [](const State &parent, const State &state) { return Node{parent, state, 0, 0}; };
            State state = startState;
            Node node = makeNode(state, state);
            nodeMap.insert({state, node});
            queue.push(node);
            do {
                if (queue.empty()) {
                    return nullopt;
                }
                node = queue.front();
                queue.pop();
                state = node.state;
                if (isGoal(state)) {
                    goalState = state;
                    return goalState;
                }
                if (!isValid(state)) {
                    continue;
                }
                Vec<Action> actions = getActions(state);
                for (const Action &action: actions) {
                    State child = transferState(state, action);
                    if (nodeMap.find(child) == nodeMap.end()) {
                        Node childNode = makeNode(state, child);
                        nodeMap.insert({child, childNode});
                        queue.push(childNode);
                    }
                }
            } while (true);
        }

        Option<State> depthFirstSearch() {
            clear();
            auto makeNode = [](const State &parent, const State &state) { return Node{parent, state, 0, 0}; };
            State state = startState;
            Node node = makeNode(state, state);
            nodeMap.insert({state, node});
            stack.push(node);
            do {
                if (stack.empty()) {
                    return nullopt;
                }
                node = stack.top();
                stack.pop();
                state = node.state;
                if (isGoal(state)) {
                    goalState = state;
                    return goalState;
                }
                if (!isValid(state)) {
                    continue;
                }
                Vec<Action> actions = getActions(state);
                for (const Action &action: actions) {
                    State child = transferState(state, action);
                    if (nodeMap.find(child) == nodeMap.end()) {
                        Node childNode = makeNode(state, child);
                        nodeMap.insert({child, childNode});
                        stack.push(childNode);
                    }
                }
            } while (true);
        }

        Option<State> uniformCostSearch() {
            clear();
            State state = startState;
            Node node{state, state, 0, 0};
            nodeMap.insert({state, node});
            priorityQueue.push(node);
            do {
                if (priorityQueue.empty()) {
                    return nullopt;
                }
                node = priorityQueue.top();
                priorityQueue.pop();
                state = node.state;
                if (isGoal(state)) {
                    goalState = state;
                    return goalState;
                }
                if (!isValid(state)) {
                    continue;
                }
                Vec<Action> actions = getActions(state);
                for (const Action &action: actions) {
                    State child = transferState(state, action);
                    if (nodeMap.find(child) == nodeMap.end()) {
                        Node childNode{state, child, g(state, action), node.depth + 1};
                        nodeMap.insert({child, childNode});
                        priorityQueue.push(childNode);
                    }
                }
            } while (true);
        }

        Option<State> greedySearch() {
            clear();
            State state = startState;
            Node node{state, state, 0, 0};
            nodeMap.insert({state, node});
            priorityQueue.push(node);
            do {
                if (priorityQueue.empty()) {
                    return nullopt;
                }
                node = priorityQueue.top();
                priorityQueue.pop();
                state = node.state;
                if (isGoal(state)) {
                    goalState = state;
                    return goalState;
                }
                if (!isValid(state)) {
                    continue;
                }
                Vec<Action> actions = getActions(state);
                for (const Action &action: actions) {
                    State child = transferState(state, action);
                    if (nodeMap.find(child) == nodeMap.end()) {
                        Node childNode{state, child, h(child), node.depth + 1};
                        nodeMap.insert({child, childNode});
                        priorityQueue.push(childNode);
                    }
                }
            } while (true);
        };

        Option<State> aStarSearch() {
            clear();
            State state = startState;
            Node node{state, state, 0, 0};
            nodeMap.insert({state, node});
            priorityQueue.push(node);
            do {
                if (priorityQueue.empty()) {
                    return nullopt;
                }
                node = priorityQueue.top();
                priorityQueue.pop();
                state = node.state;
                if (isGoal(state)) {
                    goalState = state;
                    return goalState;
                }
                if (!isValid(state)) {
                    continue;
                }
                Vec<Action> actions = getActions(state);
                for (const Action &action: actions) {
                    State child = transferState(state, action);
                    if (nodeMap.find(child) == nodeMap.end()) {
                        Node childNode{state, child, h(child) + g(state, action), node.depth + 1};
                        nodeMap.insert({child, childNode});
                        priorityQueue.push(childNode);
                    }
                }
            } while (true);
        }

        virtual void printPath() const {
            if (!goalState) {
                std::cout << "Cannot access goal state" << std::endl;
                return;
            }
            std::cout << "Path: ";
        }
    };

};

#endif //SEARCH_SEARCH_HPP
