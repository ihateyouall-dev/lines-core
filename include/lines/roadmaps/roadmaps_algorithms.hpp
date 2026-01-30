#include "roadmaps.h"
#include <cassert>
#include <ranges>
#include <stack>

namespace Lines::Roadmaps {
auto dfs(Roadmap &rmap) -> std::vector<RoadmapNode::NodePtr> {
    std::vector<RoadmapNode::NodePtr> result;
    auto root = rmap.root();

    std::stack<RoadmapNode::NodePtr> stack;
    stack.push(root);

    while (!stack.empty()) {
        const auto nptr = stack.top().lock();
        stack.pop();

        result.emplace_back(nptr);

        for (const auto &child : std::ranges::reverse_view(nptr->children())) {
            stack.push(child);
        }
    }

    return result;
}
} // namespace Lines::Roadmaps
