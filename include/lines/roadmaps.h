#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <utility>
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
    using NodePtr = RoadmapNode *;
    enum class State : uint8_t { NotCompleted, Completed, Skipped, InProgress };

  private:
    State _state = State::NotCompleted;
    const NodeID _id; // NOLINT
    RoadmapNodeInfo _info;
    NodePtr _parent;
    std::vector<NodePtr> _children;

  public:
    explicit RoadmapNode(NodeID id, RoadmapNodeInfo info, RoadmapNode *parent)
        : _id(id), _info(std::move(info)), _parent(parent) {}
    friend class Roadmap;
    RoadmapNode() = delete;
    RoadmapNode(const RoadmapNode &) = delete;
    RoadmapNode(RoadmapNode &&) = delete;
    auto operator=(const RoadmapNode &) -> RoadmapNode & = delete;
    auto operator=(RoadmapNode &&) -> RoadmapNode & = delete;
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

    void add_child(NodePtr node) {
        if (std::ranges::find(_children, node) != _children.end()) {
            throw std::invalid_argument("RoadmapNode::add_child: attempt to add existing child");
        }
        _children.emplace_back(node);
    }

    void remove_child(NodePtr child) {
        _children.erase(std::ranges::remove(_children, child).begin(), _children.end());
    }

    void set_parent(NodePtr parent) { _parent = parent; }

    [[nodiscard]] auto out_degree() const -> std::size_t { return _children.size(); }

    [[nodiscard]] auto parent() const -> NodePtr { return _parent; }
    [[nodiscard]] auto children() const -> const std::vector<NodePtr> & { return _children; }
    [[nodiscard]] auto id() const -> NodeID { return _id; }
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
    std::vector<std::unique_ptr<RoadmapNode>> nodes;

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
    static constexpr RoadmapNode::NodeID ROOT_ID = 0;
    explicit Roadmap(RoadmapInfo info) : _info(std::move(info)) {
        nodes.emplace_back(
            std::make_unique<RoadmapNode>(ROOT_ID, RoadmapNodeInfo{"Root", "Root node"}, nullptr));
    }
    Roadmap(const Roadmap &) = default;
    Roadmap(Roadmap &&) = default;
    auto operator=(const Roadmap &) -> Roadmap & = default;
    auto operator=(Roadmap &&) -> Roadmap & = default;
    ~Roadmap() = default;
    auto operator[](RoadmapNode::NodeID id) -> RoadmapNode * { return nodes[id].get(); }
    auto operator[](RoadmapNode::NodeID id) const -> const RoadmapNode * { return nodes[id].get(); }

    auto root() -> RoadmapNode * { return nodes[ROOT_ID].get(); }
    [[nodiscard]] static auto is_root(RoadmapNode::NodeID id) -> bool { return id == ROOT_ID; }

    auto last() -> RoadmapNode::NodePtr { return nodes[nodes.size() - 1].get(); }

    [[nodiscard]] auto last_id() const -> RoadmapNode::NodeID { return nodes.size() - 1; }

    auto add_node(RoadmapNode::NodePtr parent, const RoadmapNodeInfo &info)
        -> RoadmapNode::NodePtr {
        const RoadmapNode::NodeID id = free_id();
        std::unique_ptr<RoadmapNode> node = std::make_unique<RoadmapNode>(id, info, parent);
        parent->add_child(node.get());
        if (id < nodes.size()) {
            nodes[id] = std::move(node);
        } else {
            nodes.emplace_back(std::move(node));
        }
        return node.get(); // NOLINT
    }

    void remove_node(RoadmapNode::NodeID id) {
        if (id == ROOT_ID) {
            throw std::invalid_argument("Roadmap::delete_node: attempt to delete root");
        }
        auto *parent = nodes[id]->parent();
        auto *node = nodes[id].get();
        for (const auto &child : node->children()) {
            child->set_parent(parent);
            parent->add_child(child);
        }
        parent->remove_child(nodes[id].get());
        nodes[id] = nullptr;
    }

    [[nodiscard]] auto size() const -> std::size_t { return nodes.size(); }
};
}; // namespace Lines
