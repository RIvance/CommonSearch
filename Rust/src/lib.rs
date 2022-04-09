use std::collections::{HashSet, VecDeque, BinaryHeap};
use std::hash::Hash;
use std::ops::Add;
use std::rc::Rc;

pub trait State
{
    type Action;
    fn get_actions(&self) -> Vec<&Self::Action>;
    fn transfer(&self, action: &Self::Action) -> Rc<Self>;
    fn is_valid(&self) -> bool { true }
}

pub struct Problem<S, GL>
where 
    S: State + Eq + Hash,
    GL: Fn(&S) -> bool,
{
    start_state: Rc<S>,
    is_goal: GL,
}

impl<S, GL> Problem<S, GL>
where
    S: State + Eq + Hash,
    GL: Fn(&S) -> bool,
{
    pub fn new(start_state: Rc<S>, is_goal: GL) -> Self
    {
        Self { start_state, is_goal }
    }

    pub fn breadth_first_search(&self) -> Option<Rc<S>>
    {
        let mut visited = HashSet::<Rc<S>>::new();
        visited.insert(self.start_state.clone());
        let mut queue = VecDeque::<Rc<S>>::new();
        queue.push_front(self.start_state.clone());

        while !queue.is_empty() {
            let state = queue.pop_back().unwrap();
            if (self.is_goal)(state.as_ref()) && state.is_valid() {
                return Some(state);
            }
            if state.is_valid() {
                state.get_actions().into_iter().for_each(|act| {
                    let next = state.transfer(&act);
                    if visited.contains(&next) {
                        visited.insert(next.clone());
                        queue.push_front(next);
                    }
                });
            }
        }
        
        return Option::None;
    }

    pub fn depth_first_search(&self) -> Option<Rc<S>>
    {
        let mut visited = HashSet::<Rc<S>>::new();
        visited.insert(self.start_state.clone());
        let mut stack = VecDeque::<Rc<S>>::new();
        stack.push_front(self.start_state.clone());

        while !stack.is_empty() {
            let state = stack.pop_front().unwrap();
            if (self.is_goal)(state.as_ref()) && state.is_valid() {
                return Some(state);
            }
            if state.is_valid() {
                state.get_actions().into_iter().for_each(|act| {
                    let next = state.transfer(&act);
                    if visited.contains(&next) {
                        visited.insert(next.clone());
                        stack.push_front(next);
                    }
                });
            }
        }
        
        return Option::None;
    }

    pub fn uniform_cost_search<C, G>(&self, g: G) -> Option<Rc<S>>
    where
        C: Ord + Default,
        G: Fn(&S, &S::Action) -> C,
    {
        #[derive(PartialEq, Eq)]
        struct Node<S: Eq, C: Ord + Default>(Rc<S>, u32, C);

        impl<S: Eq, C: Ord + Default> PartialOrd for Node<S, C> {
            fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
                Some(self.cmp(other))
            }
        }

        impl<S: Eq, C: Ord + Default> Ord for Node<S, C> {
            fn cmp(&self, other: &Self) -> std::cmp::Ordering {
                other.2.cmp(&self.2).then_with(|| other.1.cmp(&self.1))
            }
        }

        let mut visited = HashSet::<Rc<S>>::new();
        visited.insert(self.start_state.clone());
        let mut heap = BinaryHeap::<Node<S, C>>::new();
        heap.push(Node(self.start_state.clone(), 0, C::default()));

        while !heap.is_empty() {

            let node = heap.pop().unwrap();
            let state = node.0;
            let depth = node.1;

            if (self.is_goal)(state.as_ref()) && state.is_valid() {
                return Some(state);
            }

            if state.is_valid() {
                state.get_actions().into_iter().for_each(|act| {
                    let next = state.transfer(&act);
                    if visited.contains(&next) {
                        visited.insert(next.clone());
                        let next_node = Node(next.clone(), depth + 1, g(state.as_ref(), act));
                        heap.push(next_node);
                    }
                });
            }
        }
        
        return Option::None;
    }

    pub fn greedy_search<C, H>(&self, h: H) -> Option<Rc<S>>
    where
        C: Ord + Default,
        H: Fn(&S) -> C,
    {
        #[derive(PartialEq, Eq)]
        struct Node<S: Eq, C: Ord + Default>(Rc<S>, u32, C);

        impl<S: Eq, C: Ord + Default> PartialOrd for Node<S, C> {
            fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
                Some(self.cmp(other))
            }
        }

        impl<S: Eq, C: Ord + Default> Ord for Node<S, C> {
            fn cmp(&self, other: &Self) -> std::cmp::Ordering {
                other.2.cmp(&self.2).then_with(|| other.1.cmp(&self.1))
            }
        }

        let mut visited = HashSet::<Rc<S>>::new();
        visited.insert(self.start_state.clone());
        let mut heap = BinaryHeap::<Node<S, C>>::new();
        heap.push(Node(self.start_state.clone(), 0, C::default()));

        while !heap.is_empty() {

            let node = heap.pop().unwrap();
            let state = node.0;
            let depth = node.1;

            if (self.is_goal)(state.as_ref()) && state.is_valid() {
                return Some(state);
            }

            if state.is_valid() {
                state.get_actions().into_iter().for_each(|act| {
                    let next = state.transfer(&act);
                    if visited.contains(&next) {
                        visited.insert(next.clone());
                        let next_node = Node(next.clone(), depth + 1, h(state.as_ref()));
                        heap.push(next_node);
                    }
                });
            }
        }
        
        return Option::None;
    }

    pub fn a_star_search<C, G, H>(&self, g: G, h: H) -> Option<Rc<S>>
    where
        C: Ord + Default + Add<Output = C>,
        G: Fn(&S, &S::Action) -> C,
        H: Fn(&S) -> C,
    {
        #[derive(PartialEq, Eq)]
        struct Node<S: Eq, C: Ord + Default>(Rc<S>, u32, C);

        impl<S: Eq, C: Ord + Default> PartialOrd for Node<S, C> {
            fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
                Some(self.cmp(other))
            }
        }

        impl<S: Eq, C: Ord + Default> Ord for Node<S, C> {
            fn cmp(&self, other: &Self) -> std::cmp::Ordering {
                other.2.cmp(&self.2).then_with(|| other.1.cmp(&self.1))
            }
        }

        let mut visited = HashSet::<Rc<S>>::new();
        visited.insert(self.start_state.clone());
        let mut heap = BinaryHeap::<Node<S, C>>::new();
        heap.push(Node(self.start_state.clone(), 0, C::default()));

        while !heap.is_empty() {

            let node = heap.pop().unwrap();
            let state = node.0;
            let depth = node.1;

            if (self.is_goal)(state.as_ref()) && state.is_valid() {
                return Some(state);
            }

            if state.is_valid() {
                state.get_actions().into_iter().for_each(|act| {
                    let next = state.transfer(&act);
                    if visited.contains(&next) {
                        visited.insert(next.clone());
                        let cost = g(state.as_ref(), act) + h(state.as_ref());
                        let next_node = Node(next.clone(), depth + 1, cost);
                        heap.push(next_node);
                    }
                });
            }
        }
        
        return Option::None;
    }
}