#include "ProjectReader.h"

#ifdef NDEBUG

#include "../ext/TspReader/include/Parser.h"

#endif

#include <ranges>
#include <span>
#include "utils/Utils.h"
#include "utils/Print.h"

namespace qs
{

struct Data
{
    std::vector<int32_t> weights;
    size_t               order;
};

auto getTsplibDimension(std::string_view input) -> std::optional<size_t>;
auto getDimension(std::string_view input) -> std::optional<size_t>;

auto getTsplibWeights(std::string_view input) -> std::optional<std::vector<int32_t>>;
auto getWeights(std::string_view input) -> std::optional<std::vector<int32_t>>;

auto extractWeightsFromStrings(std::span<const std::string> strings) -> std::optional<std::vector<int32_t>>;

auto getData(std::string_view input) -> std::optional<Data>
{
    const auto dimension = getTsplibDimension(input) + getDimension(input);
    if (!dimension.has_value())
    {
        return {};
    }

    const auto weights = getTsplibWeights(input) + getWeights(input);
    if (!weights.has_value())
    {
        return {};
    }
    return Data {
        weights.value(),
        dimension.value(),
    };
}

auto getTsplibDimension(std::string_view input) -> std::optional<size_t>
{
    using namespace std::string_view_literals;
    static constexpr auto dimensionTag = "DIMENSION:"sv;
    auto range = input | std::ranges::views::drop(utils::saturatedAdd(input.find(dimensionTag), dimensionTag.length()))
                       | std::ranges::views::take_while([](auto character) {
        return !std::isalpha(character);
    });

    const auto number = fp::token(fp::natural<size_t>)(std::string_view(&*range.begin(), std::ranges::distance(range)));

    if (number.has_value())
    {
        return number->first;
    }
    else
    {
        return {};
    }
}

auto getDimension(std::string_view input) -> std::optional<size_t>
{
    const auto number = fp::token(fp::natural<size_t>)(input);

    if (number.has_value())
    {
        return number->first;
    }
    else
    {
        return {};
    }
}

auto getTsplibWeights(std::string_view input) -> std::optional<std::vector<int32_t>>
{
    using namespace std::string_view_literals;
    static constexpr auto edgeWeightSectionTag = "EDGE_WEIGHT_SECTION"sv;
    auto range = input | std::ranges::views::drop(utils::saturatedAdd(input.find(edgeWeightSectionTag), edgeWeightSectionTag.length()))
                       | std::ranges::views::take_while([](auto character) {
        return !std::isalpha(character);
    });

    auto buffer = std::istringstream {std::string{std::string_view(&*range.begin(), std::ranges::distance(range))}};

    const auto split = std::vector<std::string> {std::istream_iterator<std::string>{buffer},
                                                 std::istream_iterator<std::string>{}};


    if (split.empty())
    {
        return {};
    }

    return extractWeightsFromStrings(split);
}

auto getWeights(std::string_view input) -> std::optional<std::vector<int32_t>>
{
    auto buffer = std::istringstream {std::string{input}};
    const auto split = std::vector<std::string> {std::istream_iterator<std::string>{buffer},
                                                 std::istream_iterator<std::string>{}};
    if (split.empty())
    {
        return {};
    }

    const auto weights = std::span{std::next(split.begin()), split.end()};
    return extractWeightsFromStrings(weights);
}

auto extractWeightsFromStrings(std::span<const std::string> strings) -> std::optional<std::vector<int32_t>>
{
    auto result = std::vector<int32_t>{};
    for (const auto& weight : strings)
    {
        const auto weightResult = fp::integer<int32_t>(weight);
        if (!weightResult.has_value())
        {
            return {};
        }
        result.push_back(weightResult->first);
    }
    return result;
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
    return makeGraphFromFullMatrix(data->weights, data->order);
}

}