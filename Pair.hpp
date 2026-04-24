#ifndef _PAIR_HPP_
#define _PAIR_HPP_

template<typename A, typename B>
class Pair {
public:
    A first;
    B second;

    Pair() = default;

    Pair(const A& first, const B& second)
        : first(first), second(second) {}

    Pair(const Pair& other) = default;

    Pair& operator=(const Pair& other) = default;
};

#endif // _PAIR_HPP_