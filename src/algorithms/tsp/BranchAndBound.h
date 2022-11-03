#pragma once

#include "TspAlgorithm.h"
#include "utils/ExtendedPriorityQueue.h"

#include <concepts>
#include <utility>

namespace qs::algo::tsp
{

class BranchAndBound : TspAlgorithm
{
public:
    struct NodeData;
    using MatrixGraph = std::vector<std::vector<tsplib::Graph::Weight>>;
    using StrategyFunction = std::function<bool(const NodeData&, const NodeData&)>;

    struct NodeData
    {
        Result::Path          path;
        MatrixGraph           reducedMatrix;
        tsplib::Graph::Weight cost;
        uint32_t              level;
    };

    explicit BranchAndBound(const StrategyFunction& strategy = lowestStrategy);

    [[nodiscard]]
    auto solve(const tsplib::Graph& graph) -> std::optional<Result> override;

    static constexpr auto lowestStrategy = [](const NodeData& node1, const NodeData& node2) {
        return node1.cost > node2.cost;
    };

private:
    auto makeNodeForEveryNeighbour(const NodeData& node) -> void;

    [[nodiscard]]
    static auto makeNode(const NodeData& bestNode, tsplib::Graph::Edge edge) -> NodeData;

    [[nodiscard]]
    static auto makeFirstNode(const tsplib::Graph& graph) -> NodeData;

    [[nodiscard]]
    static auto makeMatrixGraph(const tsplib::Graph& graph) -> MatrixGraph;


    [[nodiscard]]
    static auto removeEdges(MatrixGraph graph, tsplib::Graph::Edge) -> MatrixGraph;
    static auto removeColumn(MatrixGraph& graph, size_t column) -> void;
    static auto removeRow(MatrixGraph& graph, size_t column) -> void;
    static auto removeEdge(MatrixGraph& graph, tsplib::Graph::Edge) -> void;


    [[nodiscard]]
    static auto reduceMatrix(MatrixGraph graph) -> std::pair<MatrixGraph, tsplib::Graph::Weight>;

    [[nodiscard]]
    static auto reduceColumns(MatrixGraph& graph) -> tsplib::Graph::Weight;

    [[nodiscard]]
    static auto reduceRows(MatrixGraph& graph) -> tsplib::Graph::Weight;

    [[nodiscard]]
    static auto reduceColumn(MatrixGraph& graph, size_t column) -> tsplib::Graph::Weight;
    [[nodiscard]]
    static auto reduceRow(MatrixGraph& graph, size_t row) -> tsplib::Graph::Weight;


    qs::utils::ExtendedPriorityQueue<NodeData, std::vector<NodeData>, StrategyFunction> queue;
};

}
