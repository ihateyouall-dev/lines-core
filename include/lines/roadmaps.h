#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

namespace Lines {
// Meta information about a roadmap node
struct RoadmapNodeInfo {
    explicit RoadmapNodeInfo(std::string title,
                             std::optional<std::string> description = std::nullopt,
                             std::optional<std::vector<std::string>> tags = std::nullopt)
        : title(std::move(title)), description(std::move(description)), tags(std::move(tags)) {}
    std::string title;
    std::optional<std::string> description;
    std::optional<std::vector<std::string>> tags;
};

class RoadmapNode {
  public:
    using NodeID = std::size_t;
    enum class State : uint8_t { NotCompleted, Completed, Skipped, InProgress };

  private:
    State _state = State::NotCompleted;
    RoadmapNodeInfo _info;
    NodeID _parent;
    std::vector<NodeID> _children;

  public:
    RoadmapNode() = delete;
    explicit RoadmapNode(RoadmapNodeInfo info, NodeID parent)
        : _info(std::move(info)), _parent(parent) {}
    RoadmapNode(const RoadmapNode &) = default;
    RoadmapNode(RoadmapNode &&) = default;
    auto operator=(const RoadmapNode &) -> RoadmapNode & = default;
    auto operator=(RoadmapNode &&) -> RoadmapNode & = default;
    ~RoadmapNode() = default;

    [[nodiscard]] auto title() const -> const std::string & { return _info.title; }
    [[nodiscard]] auto description() const -> const std::optional<std::string> & {
        return _info.description;
    }
    [[nodiscard]] auto tags() const -> const std::optional<std::vector<std::string>> & {
        return _info.tags;
    }

    [[nodiscard]] auto state() const -> State { return _state; }

    void set_state(State state) { _state = state; }

    void add_child(NodeID child) {
        if (std::ranges::find(_children, child) == _children.end()) {
            _children.push_back(child);
        }
    }
    [[nodiscard]] auto out_degree() const -> std::size_t { return _children.size(); }

    [[nodiscard]] auto parent() const -> NodeID { return _parent; }
    [[nodiscard]] auto children() const -> const std::vector<NodeID> & { return _children; }
};

struct RoadmapInfo {
    explicit RoadmapInfo(std::string title, std::optional<std::string> description = std::nullopt,
                         std::optional<std::vector<std::string>> tags = std::nullopt)
        : title(std::move(title)), description(std::move(description)), tags(std::move(tags)) {}
    std::string title;
    std::optional<std::string> description;
    std::optional<std::vector<std::string>> tags;
};

class Roadmap {
    RoadmapInfo _info;
    std::vector<RoadmapNode> nodes;

  public:
    static constexpr RoadmapNode::NodeID ROOT_ID = 0;
    static constexpr RoadmapNode::NodeID NO_PARENT =
        std::numeric_limits<RoadmapNode::NodeID>::max();
    explicit Roadmap(RoadmapInfo info) : _info(std::move(info)) {
        nodes.emplace_back(RoadmapNodeInfo{"Root", "Root node"}, NO_PARENT);
    }
    Roadmap(const Roadmap &) = default;
    Roadmap(Roadmap &&) = default;
    auto operator=(const Roadmap &) -> Roadmap & = default;
    auto operator=(Roadmap &&) -> Roadmap & = default;
    ~Roadmap() = default;
    auto operator[](RoadmapNode::NodeID id) -> RoadmapNode & { return nodes[id]; }
    auto operator[](RoadmapNode::NodeID id) const -> const RoadmapNode & { return nodes[id]; }

    auto root() -> RoadmapNode & { return nodes[ROOT_ID]; }
    [[nodiscard]] static auto is_root(RoadmapNode::NodeID id) -> bool { return id == ROOT_ID; }

    auto last() -> RoadmapNode & { return nodes[nodes.size() - 1]; }

    [[nodiscard]] auto last_id() const -> RoadmapNode::NodeID { return nodes.size() - 1; }

    auto add_node(RoadmapNode::NodeID parent, const RoadmapNodeInfo &info) -> RoadmapNode::NodeID {
        nodes.emplace_back(info, parent);
        RoadmapNode::NodeID node_id = nodes.size() - 1;
        nodes[parent].add_child(node_id);
        return node_id;
    }

    [[nodiscard]] auto size() const -> std::size_t { return nodes.size(); }

    [[nodiscard]] auto title(RoadmapNode::NodeID id) const -> const std::string & {
        return nodes[id].title();
    }

    [[nodiscard]] auto description(RoadmapNode::NodeID id) const
        -> const std::optional<std::string> & {
        return nodes[id].description();
    }

    [[nodiscard]] auto tags(RoadmapNode::NodeID id) const
        -> const std::optional<std::vector<std::string>> & {
        return nodes[id].tags();
    }

    auto completed() -> bool {
        return std::ranges::all_of(
            nodes, [](const auto &node) { return node.state() == RoadmapNode::State::Completed; });
    }
};
}; // namespace Lines
