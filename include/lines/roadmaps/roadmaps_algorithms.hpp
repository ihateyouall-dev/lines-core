/*
  #        #  #     #  # # # #  # # # #
  #        #  # #   #  #        #
  #        #  #   # #  # # # #  # # # #
  #        #  #     #  #              #
  # # # #  #  #     #  # # # #  # # # #
  Copyright (c) 2025-2026 I.H.Y.A.D.

  Lines Project, Core library.
  This file is licensed under GNU Lesser General Public License v3.0 or later.
  See LICENSE for more information.
  SPDX-License-Identifier: LGPL-3.0-or-later.
*/
#pragma once

#include "roadmaps.hpp"
#include <cassert>
#include <queue>
#include <ranges>
#include <stack>

namespace Lines::Roadmaps {
namespace detail {
template <typename Fn> void dfs_impl(const Roadmap &rmap, const Fn &fn) {
    auto root = rmap.root();

    std::stack<RoadmapNode::NodePtr> stack;
    stack.push(root);

    while (!stack.empty()) {
        const auto node = stack.top();
        const auto nptr = node.lock();
        stack.pop();

        fn(node);

        for (const auto &child : std::ranges::reverse_view(nptr->children())) {
            stack.push(child);
        }
    }
}

template <typename Fn> void bfs_impl(const Roadmap &rmap, const Fn &fn) {
    auto root = rmap.root();

    std::queue<RoadmapNode::NodePtr> queue;

    queue.push(root);

    while (!queue.empty()) {
        const auto node = queue.front();
        const auto nptr = node.lock();

        queue.pop();

        fn(node);
        for (const auto &child : nptr->children()) {
            queue.push(child);
        }
    }
}
} // namespace detail
LINES_API auto dfs(const Roadmap &rmap) -> std::vector<RoadmapNode::NodePtr>;

LINES_API auto bfs(const Roadmap &rmap) -> std::vector<RoadmapNode::NodePtr>;

template <typename Fn> void dfs_foreach(const Roadmap &rmap, Fn &&visitor) {
    detail::dfs_impl(rmap, std::forward<Fn>(visitor));
}

template <typename Fn> void bfs_foreach(const Roadmap &rmap, Fn &&visitor) {
    detail::bfs_impl(rmap, std::forward<Fn>(visitor));
}
} // namespace Lines::Roadmaps
