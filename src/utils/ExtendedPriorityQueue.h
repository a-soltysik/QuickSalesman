#pragma once

#include <queue>
#include <algorithm>

namespace qs::utils
{

template<typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class ExtendedPriorityQueue : public std::priority_queue<T, Container, Compare>
{
public:
    using std::priority_queue<T, Container, Compare>::priority_queue;

    auto seize_top() -> T
    {
        std::ranges::pop_heap(c, comp);
        auto value = std::move(c.back());
        c.pop_back();
        return value;
    }

protected:
    using std::priority_queue<T, Container, Compare>::c;
    using std::priority_queue<T, Container, Compare>::comp;
};

}