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

#include <algorithm>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <lines/detail/macro.h>

namespace Lines {
struct LINES_API RoadmapNodeInfo {
    explicit RoadmapNodeInfo(std::string title,
                             std::optional<std::string> description = std::nullopt,
                             std::vector<std::string> tags = {})
        : title(std::move(title)), description(std::move(description)), tags(std::move(tags)) {}
    std::string title;
    std::optional<std::string> description;
    std::vector<std::string> tags;
};

class LINES_API RoadmapNode {
  public:
    using NodeID = std::size_t;
    using NodePtr = std::weak_ptr<RoadmapNode>;
    enum class State : uint8_t { NotCompleted, Completed, Skipped, InProgress };

  private:
    State _state = State::NotCompleted;
    const NodeID _id; // NOLINT
    RoadmapNodeInfo _info;
    NodePtr _parent;
    std::vector<NodePtr> _children;

  public:
    explicit RoadmapNode(NodeID id, RoadmapNodeInfo info, NodePtr parent)
        : _id(id), _info(std::move(info)), _parent(std::move(parent)) {
        if (_info.title.empty()) {
            throw std::invalid_argument("RoadmapNode: title cannot be empty");
        }
    }
    RoadmapNode() = delete;
    RoadmapNode(const RoadmapNode &) = delete;
    RoadmapNode(RoadmapNode &&) = delete;
    auto operator=(const RoadmapNode &) -> RoadmapNode & = delete;
    auto operator=(RoadmapNode &&) -> RoadmapNode & = delete;
    ~RoadmapNode() = default;

    LINES_NODISCARD auto title() const -> const std::string & { return _info.title; }
    LINES_NODISCARD auto description() const -> const std::optional<std::string> & {
        return _info.description;
    }
    LINES_NODISCARD auto tags() const -> std::vector<std::string> { return _info.tags; }

    LINES_NODISCARD auto state() const -> State { return _state; }

    void set_state(State state) { _state = state; }

    void add_child(const NodePtr &node) { _children.emplace_back(node); }

    void remove_child(const NodePtr &child) {
        _children.erase(std::ranges::remove_if(
                            _children, [&](const NodePtr &el) { return el.lock() == child.lock(); })
                            .begin(),
                        _children.end());
    }

    void set_parent(NodePtr parent) { _parent = std::move(parent); }

    LINES_NODISCARD auto out_degree() const -> std::size_t { return _children.size(); }

    LINES_NODISCARD auto parent() const -> NodePtr { return _parent; }
    LINES_NODISCARD auto children() const -> const std::vector<NodePtr> & { return _children; }
    LINES_NODISCARD auto id() const -> NodeID { return _id; }
};

struct LINES_API RoadmapInfo {
    explicit RoadmapInfo(std::string title, std::optional<std::string> description = std::nullopt,
                         std::vector<std::string> tags = {})
        : title(std::move(title)), description(std::move(description)), tags(std::move(tags)) {}
    std::string title;
    std::optional<std::string> description;
    std::vector<std::string> tags;
};

class LINES_API Roadmap {
    RoadmapInfo _info;
    std::vector<std::shared_ptr<RoadmapNode>> nodes;

    auto free_id() -> RoadmapNode::NodeID {
        RoadmapNode::NodeID id = nodes.size();
        for (size_t i = 1; i < nodes.size(); ++i) {
            if (nodes[i] == nullptr) {
                return i;
            }
        }
        return id;
    }

  public:
    static LINES_CONSTEXPR RoadmapNode::NodeID ROOT_ID = 0;
    explicit Roadmap(RoadmapInfo info) : _info(std::move(info)) {
        nodes.emplace_back(std::make_shared<RoadmapNode>(
            ROOT_ID, RoadmapNodeInfo{"Root", "Root node"}, RoadmapNode::NodePtr{}));
        if (_info.title.empty()) {
            throw std::invalid_argument("Roadmapinfo: title cannot be empty");
        }
    }
    Roadmap(const Roadmap &) = default;
    Roadmap(Roadmap &&) = default;
    auto operator=(const Roadmap &) -> Roadmap & = default;
    auto operator=(Roadmap &&) -> Roadmap & = default;
    ~Roadmap() = default;
    auto operator[](RoadmapNode::NodeID id) -> RoadmapNode::NodePtr { return nodes[id]; }

    auto root() -> RoadmapNode::NodePtr { return nodes[ROOT_ID]; }
    LINES_NODISCARD auto root() const -> RoadmapNode::NodePtr { return nodes[ROOT_ID]; }
    LINES_NODISCARD static auto is_root(RoadmapNode::NodeID id) -> bool { return id == ROOT_ID; }

    auto last() -> RoadmapNode::NodePtr { return nodes[nodes.size() - 1]; }
    LINES_NODISCARD auto last() const -> RoadmapNode::NodePtr { return nodes[nodes.size() - 1]; }

    LINES_NODISCARD auto last_id() const -> RoadmapNode::NodeID { return nodes.size() - 1; }

    auto add_node(RoadmapNode::NodePtr parent, const RoadmapNodeInfo &info)
        -> RoadmapNode::NodePtr {
        const RoadmapNode::NodeID id = free_id();
        std::shared_ptr<RoadmapNode> node = std::make_shared<RoadmapNode>(id, info, parent);
        if (auto p = parent.lock()) {
            p->add_child(node);
        }
        RoadmapNode::NodePtr node_ptr = node;
        if (id < nodes.size()) {
            nodes[id] = std::move(node);
        } else {
            nodes.emplace_back(std::move(node));
        }
        return node_ptr;
    }

    void remove_node(RoadmapNode::NodeID id) {
        if (id == ROOT_ID) {
            throw std::invalid_argument("Roadmap::delete_node: attempt to delete root");
        }
        auto parent = nodes[id]->parent();
        RoadmapNode::NodePtr node = nodes[id];
        if (auto n = node.lock()) {
            if (auto p = parent.lock()) {
                for (const auto &child : n->children()) {
                    child.lock()->set_parent(parent);
                    parent.lock()->add_child(child);
                }
            }
        }
        parent.lock()->remove_child(nodes[id]);
        nodes[id] = nullptr;
    }

    LINES_NODISCARD auto size() const -> std::size_t { return nodes.size(); }

    LINES_NODISCARD auto title() const -> std::string { return _info.title; }

    LINES_NODISCARD auto description() const -> std::optional<std::string> {
        return _info.description;
    }

    LINES_NODISCARD auto tags() const -> std::vector<std::string> { return _info.tags; }
};
}; // namespace Lines
