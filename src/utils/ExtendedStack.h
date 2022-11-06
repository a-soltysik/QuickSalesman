#pragma once

#include <stack>

namespace qs::utils
{

template<typename T, typename Container = std::deque<T>>
class ExtendedStack : public std::stack<T, Container>
{
public:
    using std::stack<T, Container>::stack;

    auto seize_top() -> T
    {
        auto value = std::move(c.back());
        c.pop_back();
        return value;
    }

protected:
    using std::stack<T, Container>::c;
};

}