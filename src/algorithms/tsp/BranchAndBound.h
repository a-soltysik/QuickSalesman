#pragma once

#include "algorithms/tsp/TspAlgorithm.h"
#include "utils/ExtendedPriorityQueue.h"
#include "utils/ExtendedStack.h"
#include "utils/Utils.h"

namespace qs::algo::tsp
{

namespace bnb
{

template<typename T>
concept Container =
requires(T c, typename T::value_type t)
{
    c.seize_top();
    { c.empty() } -> std::convertible_to<bool>;
    c.push(std::move(t));
};

struct NodeData
{
    using MatrixGraph = std::vector<std::vector<tsplib::Graph::Weight>>;

    TspResult::Path       path;
    MatrixGraph           reducedMatrix;
    tsplib::Graph::Weight cost;
    uint32_t              level;
};

constexpr auto less = [](const NodeData& node1, const NodeData& node2) {
    return node1.cost > node2.cost;
};

}

template<bnb::Container T>
class BranchAndBound : public TspAlgorithm
{
public:
    template<typename... Args>
    explicit BranchAndBound(Args&& ... args)
        : container(std::forward<Args>(args)...) { }

    [[nodiscard]]
    auto solve(const tsplib::Graph& graph) -> Result override;

protected:

    auto makeNodeForEveryNeighbour(const bnb::NodeData& node) -> void;

    [[nodiscard]]
    static auto makeNode(const bnb::NodeData& parent, tsplib::Graph::Edge edge) -> bnb::NodeData;

    [[nodiscard]]
    static auto makeFirstNode(const tsplib::Graph& graph) -> bnb::NodeData;

    [[nodiscard]]
    static auto makeMatrixGraph(const tsplib::Graph& graph) -> bnb::NodeData::MatrixGraph;


    [[nodiscard]]
    static auto removeEdges(bnb::NodeData::MatrixGraph graph, tsplib::Graph::Edge) -> bnb::NodeData::MatrixGraph;
    static auto removeColumn(bnb::NodeData::MatrixGraph& graph, size_t column) -> void;
    static auto removeRow(bnb::NodeData::MatrixGraph& graph, size_t row) -> void;
    static auto removeEdge(bnb::NodeData::MatrixGraph& graph, tsplib::Graph::Edge) -> void;


    [[nodiscard]]
    static auto
    reduceMatrix(bnb::NodeData::MatrixGraph graph) -> std::pair<bnb::NodeData::MatrixGraph, tsplib::Graph::Weight>;

    [[nodiscard]]
    static auto reduceColumns(bnb::NodeData::MatrixGraph& graph) -> tsplib::Graph::Weight;

    [[nodiscard]]
    static auto reduceRows(bnb::NodeData::MatrixGraph& graph) -> tsplib::Graph::Weight;

    [[nodiscard]]
    static auto reduceColumn(bnb::NodeData::MatrixGraph& graph, size_t column) -> tsplib::Graph::Weight;
    [[nodiscard]]
    static auto reduceRow(bnb::NodeData::MatrixGraph& graph, size_t row) -> tsplib::Graph::Weight;

    T             container;
    bnb::NodeData currentOptimum;
};

namespace bnb
{

inline const auto lowCost = BranchAndBound<
    qs::utils::ExtendedPriorityQueue<NodeData, std::vector<NodeData>, decltype(less)>> {less};

inline const auto depthFirstSearch = BranchAndBound<qs::utils::ExtendedStack<NodeData>> {};

}

template<bnb::Container T>
auto BranchAndBound<T>::solve(const tsplib::Graph& graph) -> Result
{
    qs::utils::print(std::is_trivially_copyable_v<bnb::NodeData>);
    if (!graph.isComplete())
    {
        return {};
    }

    currentOptimum = {
        .path = {},
        .reducedMatrix = {},
        .cost = tsplib::Graph::INFINITY_WEIGHT,
        .level = {}
    };

    container.push(makeFirstNode(graph));

    while (!container.empty())
    {
        auto node = container.seize_top();

        if (node.cost >= currentOptimum.cost)
        {
            continue;
        }
        if (node.level >= graph.getOrder() - 1)
        {
            node.path.push_back(0);
            currentOptimum = std::move(node);
        }
        else
        {
            makeNodeForEveryNeighbour(node);
        }
    }

    if (currentOptimum.cost == tsplib::Graph::INFINITY_WEIGHT)
    {
        return {};
    }

    return ResultValue {
        .path = std::move(currentOptimum.path),
        .distance = currentOptimum.cost,
    };
}

template<bnb::Container T>
auto BranchAndBound<T>::makeNodeForEveryNeighbour(const bnb::NodeData& node) -> void
{
    for (auto i = tsplib::Graph::Vertex {}; i < node.reducedMatrix.size(); i++)
    {
        if (node.reducedMatrix[node.path.back()][i] < currentOptimum.cost)
        {
            container.push(makeNode(node, {node.path.back(), i}));
        }
    }
}

template<bnb::Container T>
auto BranchAndBound<T>::makeNode(const bnb::NodeData& parent,
                                 tsplib::Graph::Edge edge) -> bnb::NodeData
{
    auto newPath = parent.path;
    newPath.push_back(edge.second);

    auto reducingResult = reduceMatrix(removeEdges(parent.reducedMatrix, edge));

    return {
        .path = std::move(newPath),
        .reducedMatrix = std::move(reducingResult.first),
        .cost = qs::utils::saturatedAdd({parent.cost,
                                         parent.reducedMatrix[edge.first][edge.second],
                                         reducingResult.second}),
        .level = parent.level + 1
    };
}

template<bnb::Container T>
auto BranchAndBound<T>::makeFirstNode(const tsplib::Graph& graph) -> bnb::NodeData
{
    auto reducingResult = reduceMatrix(makeMatrixGraph(graph));
    return bnb::NodeData {
        .path = {0},
        .reducedMatrix = std::move(reducingResult.first),
        .cost = reducingResult.second,
        .level = 0
    };
}

template<bnb::Container T>
auto BranchAndBound<T>::makeMatrixGraph(const tsplib::Graph& graph) -> bnb::NodeData::MatrixGraph
{
    auto matrixGraph = bnb::NodeData::MatrixGraph {};
    matrixGraph.reserve(graph.getOrder());

    for (auto i = tsplib::Graph::Vertex {}; i < graph.getOrder(); i++)
    {
        matrixGraph.emplace_back();
        matrixGraph.back().reserve(graph.getOrder());

        for (auto j = tsplib::Graph::Vertex {}; j < graph.getOrder(); j++)
        {
            matrixGraph.back().push_back(graph.getWeightUnchecked({i, j}));
        }
    }

    return matrixGraph;
}

template<bnb::Container T>
auto BranchAndBound<T>::reduceMatrix(
    bnb::NodeData::MatrixGraph graph) -> std::pair<bnb::NodeData::MatrixGraph, tsplib::Graph::Weight>
{
    const auto cost = reduceColumns(graph) + reduceRows(graph);

    return {std::move(graph), cost};
}

template<bnb::Container T>
auto BranchAndBound<T>::reduceColumns(bnb::NodeData::MatrixGraph& graph) -> tsplib::Graph::Weight
{
    auto fullCost = tsplib::Graph::Weight {};

    for (auto i = tsplib::Graph::Vertex {}; i < graph.size(); i++)
    {
        fullCost += reduceColumn(graph, i);
    }
    return fullCost;
}

template<bnb::Container T>
auto BranchAndBound<T>::reduceRows(bnb::NodeData::MatrixGraph& graph) -> tsplib::Graph::Weight
{
    auto fullCost = tsplib::Graph::Weight {};

    for (auto i = tsplib::Graph::Vertex {}; i < graph.size(); i++)
    {
        fullCost += reduceRow(graph, i);
    }
    return fullCost;
}

template<bnb::Container T>
auto BranchAndBound<T>::reduceColumn(bnb::NodeData::MatrixGraph& graph, size_t column) -> tsplib::Graph::Weight
{
    auto cost = tsplib::Graph::INFINITY_WEIGHT;

    for (auto i = tsplib::Graph::Vertex {}; i < graph.size(); i++)
    {
        cost = std::min(cost, graph[i][column]);
    }

    if (cost == tsplib::Graph::INFINITY_WEIGHT)
    {
        return 0;
    }

    for (auto i = tsplib::Graph::Vertex {}; i < graph.size(); i++)
    {
        if (graph[i][column] != tsplib::Graph::INFINITY_WEIGHT)
        {
            graph[i][column] -= cost;
        }
    }
    return cost;
}

template<bnb::Container T>
auto BranchAndBound<T>::reduceRow(bnb::NodeData::MatrixGraph& graph, size_t row) -> tsplib::Graph::Weight
{
    const auto cost = *std::ranges::min_element(graph[row]);

    if (cost == tsplib::Graph::INFINITY_WEIGHT)
    {
        return 0;
    }

    for (auto i = tsplib::Graph::Vertex {}; i < graph.size(); i++)
    {
        if (graph[row][i] != tsplib::Graph::INFINITY_WEIGHT)
        {
            graph[row][i] -= cost;
        }
    }
    return cost;
}

template<bnb::Container T>
auto
BranchAndBound<T>::removeEdges(bnb::NodeData::MatrixGraph graph, tsplib::Graph::Edge edge) -> bnb::NodeData::MatrixGraph
{
    removeRow(graph, edge.first);
    removeColumn(graph, edge.second);
    removeEdge(graph, {edge.second, edge.first});

    return graph;
}

template<bnb::Container T>
auto BranchAndBound<T>::removeColumn(bnb::NodeData::MatrixGraph& graph, size_t column) -> void
{
    for (auto i = tsplib::Graph::Vertex {}; i < graph.size(); i++)
    {
        graph[i][column] = tsplib::Graph::INFINITY_WEIGHT;
    }
}

template<bnb::Container T>
auto BranchAndBound<T>::removeRow(bnb::NodeData::MatrixGraph& graph, size_t row) -> void
{
    std::ranges::fill(graph[row], tsplib::Graph::INFINITY_WEIGHT);
}

template<bnb::Container T>
auto BranchAndBound<T>::removeEdge(bnb::NodeData::MatrixGraph& graph, tsplib::Graph::Edge edge) -> void
{
    graph[edge.first][edge.second] = tsplib::Graph::INFINITY_WEIGHT;
}


}
