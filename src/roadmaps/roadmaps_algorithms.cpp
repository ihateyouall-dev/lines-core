#include <lines/roadmaps/roadmaps_algorithms.hpp>

auto Lines::Roadmaps::dfs(const Roadmap &rmap) -> std::vector<RoadmapNode::NodePtr> {
    std::vector<RoadmapNode::NodePtr> result;
    detail::dfs_impl(rmap,
                     [&result](const RoadmapNode::NodePtr &node) { result.emplace_back(node); });
    return result;
}

auto Lines::Roadmaps::bfs(const Roadmap &rmap) -> std::vector<RoadmapNode::NodePtr> {
    std::vector<RoadmapNode::NodePtr> result;
    detail::bfs_impl(rmap,
                     [&result](const RoadmapNode::NodePtr &node) { result.emplace_back(node); });
    return result;
}
