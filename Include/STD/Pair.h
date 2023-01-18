#ifndef CAVE_STD_PAIR_H
#define CAVE_STD_PAIR_H

#include <cstddef> // size_t
#include <utility> // std::move, std::forward


namespace cave {
    template <typename T1, typename T2>
    class Pair {
    public:
        T1 first;
        T2 second;

        Pair() : first(), second() {}
        Pair(const T1& first, const T2& second) : first(first), second(second) {}
        Pair(T1&& first, T2&& second) : first(std::move(first)), second(std::move(second)) {}
        virtual ~Pair() {}

        Pair& operator=(const Pair& other) {
            first = other.first;
            second = other.second;
            return *this;
        }

        Pair& operator=(Pair&& other) {
            first = std::move(other.first);
            second = std::move(other.second);
            return *this;
        }

        bool operator==(const Pair& other) const {
            return first == other.first && second == other.second;
        }

        bool operator!=(const Pair& other) const {
            return !(*this == other);
        }
    };
}

#endif // !CAVE_STD_PAIR_H