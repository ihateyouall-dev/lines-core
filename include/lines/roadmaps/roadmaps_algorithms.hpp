#include "roadmaps.h"
#include <cassert>
#include <queue>
#include <ranges>
#include <stack>

namespace Lines::Roadmaps {
auto dfs(const Roadmap &rmap) -> std::vector<RoadmapNode::NodePtr> {
    std::vector<RoadmapNode::NodePtr> result;
    auto root = rmap.root();

    std::stack<RoadmapNode::NodePtr> stack;
    stack.push(root);

    while (!stack.empty()) {
        const auto node = stack.top();
        const auto nptr = node.lock();
        stack.pop();

        result.emplace_back(node);

        for (const auto &child : std::ranges::reverse_view(nptr->children())) {
            stack.push(child);
        }
    }

    return result;
}

auto bfs(const Roadmap &rmap) -> std::vector<RoadmapNode::NodePtr> {
    std::vector<RoadmapNode::NodePtr> result;
    std::vector<bool> visited(rmap.size(), false);

    auto root = rmap.root();

    std::queue<RoadmapNode::NodePtr> queue;

    visited[Roadmap::ROOT_ID] = true;
    queue.push(root);

    while (!queue.empty()) {
        const auto node = queue.front();
        const auto nptr = node.lock();

        queue.pop();

        result.emplace_back(node);

        for (const auto &child : nptr->children()) {
            const auto cptr = child.lock();
            if (!visited[cptr->id()]) {
                visited[cptr->id()] = true;
                queue.push(child);
            }
        }
    }

    return result;
}
} // namespace Lines::Roadmaps
