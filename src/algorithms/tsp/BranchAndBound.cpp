#include "BranchAndBound.h"

namespace qs::algo::tsp
{

BranchAndBound::BranchAndBound(const StrategyFunction& strategy)
    : queue {strategy}
{

}

auto BranchAndBound::calculate(const tsplib::Graph& graph) -> std::optional<Result>
{
    if (graph.getOrder() == 0)
    {
        return {};
    }

    queue.push(makeFirstNode(graph));

    while (!queue.empty())
    {
        auto node = queue.seize_top();

        if (node.level == graph.getOrder() - 1)
        {
            node.path.push_back(0);
            return Result {
                .path = std::move(node.path),
                .distance = node.cost,
            };
        }

        makeNodeForEveryNeighbour(node);
    }

    return {};
}

auto BranchAndBound::makeNode(const BranchAndBound::NodeData& bestNode,
                              tsplib::Graph::Edge edge) -> NodeData
{
    auto newPath = bestNode.path;
    newPath.push_back(edge.second);

    auto reducingResult = reduceMatrix(removeEdges(bestNode.reducedMatrix, edge));

    return {
        .path = std::move(newPath),
        .reducedMatrix = std::move(reducingResult.first),
        .cost = bestNode.cost + bestNode.reducedMatrix[edge.first][edge.second] + reducingResult.second,
        .level = bestNode.level + 1
    };

}

auto BranchAndBound::makeFirstNode(const tsplib::Graph& graph) -> BranchAndBound::NodeData
{
    auto reducingResult = reduceMatrix(makeMatrixGraph(graph));
    return NodeData {
        .path = {0},
        .reducedMatrix = std::move(reducingResult.first),
        .cost = reducingResult.second,
        .level = 0
    };
}

auto BranchAndBound::makeMatrixGraph(const tsplib::Graph& graph) -> MatrixGraph
{
    auto matrixGraph = MatrixGraph {};
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

auto BranchAndBound::reduceMatrix(MatrixGraph graph) -> std::pair<MatrixGraph, tsplib::Graph::Weight>
{
    const auto cost = reduceColumns(graph) + reduceRows(graph);

    return {std::move(graph), cost};
}

auto BranchAndBound::reduceColumns(MatrixGraph& graph) -> tsplib::Graph::Weight
{
    auto fullCost = tsplib::Graph::Weight {};

    for (auto i = tsplib::Graph::Vertex {}; i < graph.size(); i++)
    {
        fullCost += reduceColumn(graph, i);
    }
    return fullCost;
}

auto BranchAndBound::reduceRows(MatrixGraph& graph) -> tsplib::Graph::Weight
{
    auto fullCost = tsplib::Graph::Weight {};

    for (auto i = tsplib::Graph::Vertex {}; i < graph.size(); i++)
    {
        fullCost += reduceRow(graph, i);
    }
    return fullCost;
}

auto BranchAndBound::reduceColumn(MatrixGraph& graph, size_t column) -> tsplib::Graph::Weight
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

auto BranchAndBound::reduceRow(MatrixGraph& graph, size_t row) -> tsplib::Graph::Weight
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

auto BranchAndBound::removeEdges(MatrixGraph graph, tsplib::Graph::Edge edge) -> MatrixGraph
{
    removeRow(graph, edge.first);
    removeColumn(graph, edge.second);
    removeEdge(graph, {edge.second, edge.first});

    return graph;
}

auto BranchAndBound::removeColumn(MatrixGraph& graph, size_t column) -> void
{
    for (auto i = tsplib::Graph::Vertex {}; i < graph.size(); i++)
    {
        graph[i][column] = tsplib::Graph::INFINITY_WEIGHT;
    }
}

auto BranchAndBound::removeRow(MatrixGraph& graph, size_t row) -> void
{
    std::ranges::fill(graph[row], tsplib::Graph::INFINITY_WEIGHT);
}

auto BranchAndBound::removeEdge(MatrixGraph& graph, tsplib::Graph::Edge edge) -> void
{
    graph[edge.first][edge.second] = tsplib::Graph::INFINITY_WEIGHT;
}

auto BranchAndBound::makeNodeForEveryNeighbour(const NodeData& node) -> void
{
    for (auto i = tsplib::Graph::Vertex {}; i < node.reducedMatrix.size(); i++)
    {
        if (node.reducedMatrix[node.path.back()][i] != tsplib::Graph::INFINITY_WEIGHT)
        {
            queue.push(makeNode(node, {node.path.back(), i}));
        }
    }
}


}
