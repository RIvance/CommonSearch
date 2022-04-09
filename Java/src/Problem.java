import java.util.*;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Supplier;

public class Problem<Action, Cost extends Comparable<Cost>> {

    protected BiFunction<State<Action>, Action, Cost> costFunction;
    protected Function<State<Action>, Cost> heuristicFunction;
    protected Function<State<Action>, Boolean> isGoal;
    protected final Supplier<Cost> initialCost;
    protected final BiFunction<Cost, Cost, Cost> costAddition;

    protected State<Action> startState;

    public Problem(Supplier<Cost> initialCost, BiFunction<Cost, Cost, Cost> costAddition) {
        this.initialCost = initialCost;
        this.costAddition = costAddition;
    }

    public interface State<Action> {
        Set<Action> getActions();
        State<Action> transfer(Action action);

        default boolean isValid() {
            return true;
        }
    }

    protected class Node implements Comparable<Node> {
        public final State<Action> state;
        public final int depth;
        public final Cost cost;

        @Override
        public int compareTo(Node another) {
            return cost.compareTo(another.cost);
        }

        public Node(State<Action> state, int depth, Cost cost) {
            this.state = state;
            this.depth = depth;
            this.cost = cost;
        }

        public Node(State<Action> state) {
            this(state, 0, initialCost.get());
        }
    }

    public void setCostFunction(BiFunction<State<Action>, Action, Cost> costFunction) {
        this.costFunction = costFunction;
    }

    public void setHeuristicFunction(Function<State<Action>, Cost> heuristicFunction) {
        this.heuristicFunction = heuristicFunction;
    }

    public void setStartState(State<Action> startState) {
        this.startState = startState;
    }

    public void setGoal(Function<State<Action>, Boolean> isGoal) {
        this.isGoal = isGoal;
    }

    Optional<State<Action>> breadthFirstSearch() {

        assert startState != null;

        Set<State<Action>> visited = new HashSet<>();
        visited.add(startState);
        Queue<Node> queue = new ArrayDeque<>();
        queue.offer(new Node(startState));

        while (!queue.isEmpty()) {

            State<Action> state = queue.poll().state;

            if (isGoal.apply(state) && state.isValid()) {
                return Optional.of(state);
            }

            if (state.isValid()) {
                state.getActions().forEach(action -> {
                    State<Action> nextState = state.transfer(action);
                    if (visited.contains(nextState)) {
                        visited.add(nextState);
                        Node child = new Node(nextState);
                        queue.offer(child);
                    }
                });
            }
        }

        return Optional.empty();
    }

    Optional<State<Action>> depthFirstSearch() {

        assert startState != null;

        Set<State<Action>> visited = new HashSet<>();
        visited.add(startState);
        Stack<Node> stack = new Stack<>();
        stack.push(new Node(startState));

        while (!stack.isEmpty()) {

            State<Action> state = stack.pop().state;

            if (isGoal.apply(state) && state.isValid()) {
                return Optional.of(state);
            }

            if (state.isValid()) {
                state.getActions().forEach(action -> {
                    State<Action> nextState = state.transfer(action);
                    if (visited.contains(nextState)) {
                        visited.add(nextState);
                        Node child = new Node(nextState);
                        stack.push(child);
                    }
                });
            }
        }

        return Optional.empty();
    }

    Optional<State<Action>> uniformCostSearch() {

        assert startState != null;

        Queue<Node> priorityQueue = new PriorityQueue<>();
        priorityQueue.offer(new Node(startState));
        Set<State<Action>> visited = new HashSet<>();
        visited.add(startState);

        while (!priorityQueue.isEmpty()) {

            Node node = priorityQueue.poll();
            State<Action> state = node.state;

            if (isGoal.apply(state) && state.isValid()) {
                return Optional.of(state);
            }

            if (state.isValid()) {
                state.getActions().forEach(action -> {
                    State<Action> nextState = state.transfer(action);
                    if (visited.contains(nextState)) {
                        visited.add(nextState);
                        Cost cost = costFunction.apply(state, action);
                        Node child = new Node(nextState, node.depth + 1, cost);
                        priorityQueue.offer(child);
                    }
                });
            }
        }

        return Optional.empty();
    }

    Optional<State<Action>> greedySearch() {

        assert startState != null;

        Queue<Node> priorityQueue = new PriorityQueue<>();
        priorityQueue.offer(new Node(startState));
        Set<State<Action>> visited = new HashSet<>();
        visited.add(startState);

        while (!priorityQueue.isEmpty()) {

            Node node = priorityQueue.poll();
            State<Action> state = node.state;

            if (isGoal.apply(state) && state.isValid()) {
                return Optional.of(state);
            }

            if (state.isValid()) {
                state.getActions().forEach(action -> {
                    State<Action> nextState = state.transfer(action);
                    if (visited.contains(nextState)) {
                        visited.add(nextState);
                        Cost cost = heuristicFunction.apply(state);
                        Node child = new Node(nextState, node.depth + 1, cost);
                        priorityQueue.offer(child);
                    }
                });
            }
        }

        return Optional.empty();
    }

    Optional<State<Action>> aStarSearch() {

        assert startState != null;

        Queue<Node> priorityQueue = new PriorityQueue<>();
        priorityQueue.offer(new Node(startState));
        Set<State<Action>> visited = new HashSet<>();
        visited.add(startState);

        while (!priorityQueue.isEmpty()) {

            Node node = priorityQueue.poll();
            State<Action> state = node.state;

            if (isGoal.apply(state) && state.isValid()) {
                return Optional.of(state);
            }

            if (state.isValid()) {
                state.getActions().forEach(action -> {

                    State<Action> nextState = state.transfer(action);

                    if (visited.contains(nextState)) {
                        visited.add(nextState);

                        Cost cost = costAddition.apply(
                            costFunction.apply(state, action),
                            heuristicFunction.apply(state)
                        );

                        Node child = new Node(nextState, node.depth + 1, cost);
                        priorityQueue.offer(child);
                    }
                });
            }
        }

        return Optional.empty();
    }
}
