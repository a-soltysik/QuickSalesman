#include "ProjectReader.h"
#include "Parser.h"

namespace qs
{

#ifndef NDEBUG

struct Data
{
    std::vector<int32_t> weights;
    size_t               order;
};

template<std::unsigned_integral T>
auto tokenizedNatural(std::string_view input) -> fp::Result<T>
{
    return fp::sequence([](auto number) { return number; },
                        fp::tokenLeft(fp::natural<T>)
    )(input);
}

template<std::integral T>
auto tokenizedIntegers(std::string_view input) -> fp::Result<std::vector<T>>
{
    return fp::sequence([](const auto& integers) { return integers; },
                        fp::some(fp::tokenLeft(fp::integer<T>))
    )(input);
}

auto getData(std::string_view input) -> fp::Result<Data>
{
    return fp::sequence([](auto order, const auto& weights) { return Data {weights, order}; },
                        tokenizedNatural<size_t>,
                        tokenizedIntegers<int32_t>
    )(input);
}

auto makeGraphFromFullMatrix(const std::vector<int32_t>& weights, size_t order) -> std::optional<tsplib::Graph>
{
    if (order * order != weights.size())
    {
        return {};
    }

    auto result = tsplib::Graph{order};

    auto i = size_t {};
    auto j = size_t {};
    for (auto weight : weights)
    {
        result.addEdge({{j, i}, weight});

        if (++i == order)
        {
            i = 0;
            j++;
        }
    }

    return result;
}

auto getGraph(std::string_view input) -> std::optional<tsplib::Graph>
{
    auto data = getData(input);
    if (!data.has_value())
    {
        return {};
    }
    return makeGraphFromFullMatrix(data->first.weights, data->first.order);
}

#else

auto getGraph(std::string_view input) -> std::optional<tsplib::Graph>
{
    return {};
}

#endif


}