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

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <lines/detail/macro.h>

namespace Lines {
struct LINES_API RoadmapNodeInfo {
    explicit RoadmapNodeInfo(std::string title,
                             std::optional<std::string> description = std::nullopt,
                             std::vector<std::string> tags = {});
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
    explicit RoadmapNode(NodeID id, RoadmapNodeInfo info, NodePtr parent);
    RoadmapNode() = delete;
    RoadmapNode(const RoadmapNode &) = delete;
    RoadmapNode(RoadmapNode &&) = delete;
    auto operator=(const RoadmapNode &) -> RoadmapNode & = delete;
    auto operator=(RoadmapNode &&) -> RoadmapNode & = delete;
    ~RoadmapNode() = default;

    LINES_NODISCARD auto title() const -> const std::string &;
    LINES_NODISCARD auto description() const -> const std::optional<std::string> &;
    LINES_NODISCARD auto tags() const -> std::vector<std::string>;

    LINES_NODISCARD auto state() const -> State;

    void set_state(State state);

    void add_child(const NodePtr &node);

    void remove_child(const NodePtr &child);

    void set_parent(NodePtr parent);

    LINES_NODISCARD auto out_degree() const -> std::size_t;

    LINES_NODISCARD auto parent() const -> NodePtr;
    LINES_NODISCARD auto children() const -> const std::vector<NodePtr> &;
    LINES_NODISCARD auto id() const -> NodeID;
};

struct LINES_API RoadmapInfo {
    explicit RoadmapInfo(std::string title, std::optional<std::string> description = std::nullopt,
                         std::vector<std::string> tags = {});
    std::string title;
    std::optional<std::string> description;
    std::vector<std::string> tags;
};

class LINES_API Roadmap {
    RoadmapInfo _info;
    std::vector<std::shared_ptr<RoadmapNode>> nodes;

    auto free_id() -> RoadmapNode::NodeID;

  public:
    static LINES_CONSTEXPR RoadmapNode::NodeID ROOT_ID = 0;
    explicit Roadmap(RoadmapInfo info);
    Roadmap(const Roadmap &) = default;
    Roadmap(Roadmap &&) = default;
    auto operator=(const Roadmap &) -> Roadmap & = default;
    auto operator=(Roadmap &&) -> Roadmap & = default;
    ~Roadmap() = default;
    auto operator[](RoadmapNode::NodeID id) -> RoadmapNode::NodePtr;

    auto root() -> RoadmapNode::NodePtr;
    LINES_NODISCARD auto root() const -> RoadmapNode::NodePtr;
    LINES_NODISCARD static auto is_root(RoadmapNode::NodeID id) -> bool;

    auto last() -> RoadmapNode::NodePtr;

    LINES_NODISCARD auto last_id() const -> RoadmapNode::NodeID;

    auto add_node(RoadmapNode::NodePtr parent, const RoadmapNodeInfo &info) -> RoadmapNode::NodePtr;

    void remove_node(RoadmapNode::NodeID id);

    LINES_NODISCARD auto size() const -> std::size_t;

    LINES_NODISCARD auto title() const -> std::string;

    LINES_NODISCARD auto description() const -> std::optional<std::string>;

    LINES_NODISCARD auto tags() const -> std::vector<std::string>;
};
}; // namespace Lines
