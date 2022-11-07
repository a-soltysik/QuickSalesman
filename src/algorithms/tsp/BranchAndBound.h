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
    using MatrixGraph = std::unique_ptr<std::unique_ptr<tsplib::Graph::Weight[]>[]>;
    using Path = std::unique_ptr<tsplib::Graph::Vertex[]>;

    Path                  path;
    MatrixGraph           reducedMatrix;
    tsplib::Graph::Weight cost;
    uint32_t              level;
};

constexpr auto less = [](const NodeData& node1, const NodeData& node2) {
    return node1.cost > node2.cost;
};

}

template<bnb::Container T, typename... Args>
class BranchAndBound : public TspAlgorithm
{
public:
    explicit BranchAndBound(const Args&... args)
        : containerGetter([args...](){return T{args...};})
    {}

    [[nodiscard]]
    auto solve(const tsplib::Graph& graph) -> Result override;

private:

    auto makeNodeForEveryNeighbour(T& container, const bnb::NodeData& node, const bnb::NodeData& optimum) -> void;

    [[nodiscard]]
    auto makeNode(const bnb::NodeData& parent, tsplib::Graph::Edge edge) -> bnb::NodeData;

    [[nodiscard]]
    auto makeFirstNode(const tsplib::Graph& graph) -> bnb::NodeData;

    [[nodiscard]]
    auto makeMatrixGraph(const tsplib::Graph& graph) -> bnb::NodeData::MatrixGraph;

    [[nodiscard]]
    auto makeMatrixGraph() -> bnb::NodeData::MatrixGraph;

    [[nodiscard]]
    auto copyPath(const bnb::NodeData::Path& path) -> bnb::NodeData::Path;

    [[nodiscard]]
    auto copyMatrix(const bnb::NodeData::MatrixGraph& matrix) -> bnb::NodeData::MatrixGraph;


    [[nodiscard]]
    auto removeEdges(bnb::NodeData::MatrixGraph graph, tsplib::Graph::Edge) -> bnb::NodeData::MatrixGraph;
    auto removeColumn(bnb::NodeData::MatrixGraph& graph, size_t column) -> void;
    auto removeRow(bnb::NodeData::MatrixGraph& graph, size_t row) -> void;
    auto removeEdge(bnb::NodeData::MatrixGraph& graph, tsplib::Graph::Edge) -> void;


    [[nodiscard]]
    auto reduceMatrix(bnb::NodeData::MatrixGraph graph) -> std::pair<bnb::NodeData::MatrixGraph, tsplib::Graph::Weight>;

    [[nodiscard]]
    auto reduceColumns(bnb::NodeData::MatrixGraph& graph) -> tsplib::Graph::Weight;

    [[nodiscard]]
    auto reduceRows(bnb::NodeData::MatrixGraph& graph) -> tsplib::Graph::Weight;

    [[nodiscard]]
    auto reduceColumn(bnb::NodeData::MatrixGraph& graph, size_t column) -> tsplib::Graph::Weight;
    [[nodiscard]]
    auto reduceRow(bnb::NodeData::MatrixGraph& graph, size_t row) -> tsplib::Graph::Weight;

    std::function<T()> containerGetter;
    tsplib::Graph::Vertex graphOrder{};
};

namespace bnb
{

inline const auto lowCost = BranchAndBound<qs::utils::ExtendedPriorityQueue<NodeData, std::vector<NodeData>, decltype(less)>,
                                           decltype(less)> {less};

inline const auto depthFirstSearch = BranchAndBound<qs::utils::ExtendedStack<NodeData>>{};

}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::solve(const tsplib::Graph& graph) -> Result
{
    if (!graph.isComplete())
    {
        return {};
    }

    graphOrder = graph.getOrder();

    qs::utils::print("A");

    auto optimum = bnb::NodeData {
        .path = bnb::NodeData::Path(new tsplib::Graph::Vertex[graph.getOrder() + 1]),
        .reducedMatrix = makeMatrixGraph(),
        .cost = tsplib::Graph::INFINITY_WEIGHT,
        .level = {}
    };

    qs::utils::print("B");

    auto container = containerGetter();
    container.push(makeFirstNode(graph));

    while (!container.empty())
    {
        auto node = container.seize_top();
        //qs::utils::print(node.level);

        if (node.cost >= optimum.cost)
        {
            continue;
        }
        if (node.level >= graph.getOrder() - 1)
        {
            qs::utils::print(node.level);
            node.path[graph.getOrder()] = 0;
            optimum = std::move(node);
        }
        else
        {
            makeNodeForEveryNeighbour(container, node, optimum);
        }
    }

    if (optimum.cost == tsplib::Graph::INFINITY_WEIGHT)
    {
        return {};
    }

    return ResultValue {
        .path = TspResult::Path(optimum.path.get(), optimum.path.get() + graphOrder + 1),
        .distance = optimum.cost,
    };
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::makeNodeForEveryNeighbour(T& container, const bnb::NodeData& node, const bnb::NodeData& optimum) -> void
{
    for (auto i = tsplib::Graph::Vertex {}; i < graphOrder; i++)
    {
        if (node.reducedMatrix[node.level][i] < optimum.cost)
        {
            container.push(makeNode(node, {node.path[node.level], i}));
        }
    }
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::makeNode(const bnb::NodeData& parent,
                                 tsplib::Graph::Edge edge) -> bnb::NodeData
{
    auto newPath = copyPath(parent.path);
    newPath[parent.level + 1] = edge.second;

    auto reducingResult = reduceMatrix(removeEdges(copyMatrix(parent.reducedMatrix), edge));

    return {
        .path = std::move(newPath),
        .reducedMatrix = std::move(reducingResult.first),
        .cost = qs::utils::saturatedAdd({parent.cost,
                                         parent.reducedMatrix[edge.first][edge.second],
                                         reducingResult.second}),
        .level = parent.level + 1
    };
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::makeFirstNode(const tsplib::Graph& graph) -> bnb::NodeData
{
    auto reducingResult = reduceMatrix(makeMatrixGraph(graph));
    auto path = bnb::NodeData::Path(new tsplib::Graph::Vertex[graph.getOrder() + 1]);
    path[0] = 0;
    return bnb::NodeData {
        .path = std::move(path),
        .reducedMatrix = std::move(reducingResult.first),
        .cost = reducingResult.second,
        .level = 0
    };
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::makeMatrixGraph(const tsplib::Graph& graph) -> bnb::NodeData::MatrixGraph
{
    auto matrixGraph = makeMatrixGraph();

    for (auto i = tsplib::Graph::Vertex {}; i < graph.getOrder(); i++)
    {
        for (auto j = tsplib::Graph::Vertex {}; j < graph.getOrder(); j++)
        {
            matrixGraph[i][j] = graph.getWeightUnchecked({i, j});
        }
    }

    return matrixGraph;
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::reduceMatrix(
    bnb::NodeData::MatrixGraph graph) -> std::pair<bnb::NodeData::MatrixGraph, tsplib::Graph::Weight>
{
    const auto cost = reduceColumns(graph) + reduceRows(graph);

    return {std::move(graph), cost};
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::reduceColumns(bnb::NodeData::MatrixGraph& graph) -> tsplib::Graph::Weight
{
    auto fullCost = tsplib::Graph::Weight {};

    for (auto i = tsplib::Graph::Vertex {}; i < graphOrder; i++)
    {
        fullCost += reduceColumn(graph, i);
    }
    return fullCost;
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::reduceRows(bnb::NodeData::MatrixGraph& graph) -> tsplib::Graph::Weight
{
    auto fullCost = tsplib::Graph::Weight {};

    for (auto i = tsplib::Graph::Vertex {}; i < graphOrder; i++)
    {
        fullCost += reduceRow(graph, i);
    }
    return fullCost;
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::reduceColumn(bnb::NodeData::MatrixGraph& graph, size_t column) -> tsplib::Graph::Weight
{
    auto cost = tsplib::Graph::INFINITY_WEIGHT;

    for (auto i = tsplib::Graph::Vertex {}; i < graphOrder; i++)
    {
        cost = std::min(cost, graph[i][column]);
    }

    if (cost == tsplib::Graph::INFINITY_WEIGHT)
    {
        return 0;
    }

    for (auto i = tsplib::Graph::Vertex {}; i < graphOrder; i++)
    {
        if (graph[i][column] != tsplib::Graph::INFINITY_WEIGHT)
        {
            graph[i][column] -= cost;
        }
    }
    return cost;
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::reduceRow(bnb::NodeData::MatrixGraph& graph, size_t row) -> tsplib::Graph::Weight
{
    auto cost = tsplib::Graph::INFINITY_WEIGHT;

    for (auto i = tsplib::Graph::Vertex {}; i < graphOrder; i++)
    {
        cost = std::min(cost, graph[row][i]);
    }

    if (cost == tsplib::Graph::INFINITY_WEIGHT)
    {
        return 0;
    }

    for (auto i = tsplib::Graph::Vertex {}; i < graphOrder; i++)
    {
        if (graph[row][i] != tsplib::Graph::INFINITY_WEIGHT)
        {
            graph[row][i] -= cost;
        }
    }
    return cost;
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::removeEdges(bnb::NodeData::MatrixGraph graph, tsplib::Graph::Edge edge) -> bnb::NodeData::MatrixGraph
{
    removeRow(graph, edge.first);
    removeColumn(graph, edge.second);
    removeEdge(graph, {edge.second, edge.first});

    return graph;
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::removeColumn(bnb::NodeData::MatrixGraph& graph, size_t column) -> void
{
    for (auto i = tsplib::Graph::Vertex {}; i < graphOrder; i++)
    {
        graph[i][column] = tsplib::Graph::INFINITY_WEIGHT;
    }
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::removeRow(bnb::NodeData::MatrixGraph& graph, size_t row) -> void
{
    for (auto i = tsplib::Graph::Vertex {}; i < graphOrder; i++)
    {
        graph[row][i] = tsplib::Graph::INFINITY_WEIGHT;
    }
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::removeEdge(bnb::NodeData::MatrixGraph& graph, tsplib::Graph::Edge edge) -> void
{
    graph[edge.first][edge.second] = tsplib::Graph::INFINITY_WEIGHT;
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::makeMatrixGraph() -> bnb::NodeData::MatrixGraph
{
    auto reducedMatrix = bnb::NodeData::MatrixGraph(new std::unique_ptr<tsplib::Graph::Weight[]>[graphOrder]);
    for (auto i = size_t{}; i < graphOrder; i++)
    {
        reducedMatrix[i] = std::unique_ptr<tsplib::Graph::Weight[]>(new tsplib::Graph::Weight[graphOrder]);
    }
    return reducedMatrix;
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::copyPath(const bnb::NodeData::Path& path) -> bnb::NodeData::Path
{
    auto result = bnb::NodeData::Path(new tsplib::Graph::Vertex[graphOrder + 1]);
    std::copy(path.get(), path.get() + graphOrder + 1, result.get());
    return result;
}

template<bnb::Container T, typename... Args>
auto BranchAndBound<T, Args...>::copyMatrix(const bnb::NodeData::MatrixGraph& matrix) -> bnb::NodeData::MatrixGraph
{
    auto result = makeMatrixGraph();
    for (auto i = size_t{}; i < graphOrder; i++)
    {
        std::copy(matrix[i].get(), matrix[i].get() + graphOrder, result[i].get());
    }
    return result;
}

}
