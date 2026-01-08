#pragma once

#include <algorithm>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

namespace Lines {
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

  private:
    RoadmapNodeInfo _info;
    std::vector<NodeID> _parents;
    std::vector<NodeID> _next;

  public:
    RoadmapNode() = delete;
    explicit RoadmapNode(RoadmapNodeInfo info, std::vector<NodeID> parents = {},
                         std::vector<NodeID> next = {})
        : _info(std::move(info)), _parents(std::move(parents)), _next(std::move(next)) {}
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

    void add_parent(NodeID parent) {
        if (std::ranges::find(_parents, parent) == _parents.end()) {
            _parents.push_back(parent);
        }
    }
    void add_next(NodeID next) {
        if (std::ranges::find(_next, next) == _next.end()) {
            _next.push_back(next);
        }
    }
    [[nodiscard]] auto out_degree() const -> std::size_t { return _next.size(); }
    [[nodiscard]] auto in_degree() const -> std::size_t { return _parents.size(); }

    [[nodiscard]] auto parents() const -> const std::vector<NodeID> & { return _parents; }
    [[nodiscard]] auto next() const -> const std::vector<NodeID> & { return _next; }
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
    auto dfs_impl(RoadmapNode::NodeID current, RoadmapNode::NodeID target, // NOLINT
                  std::vector<bool> &visited, std::vector<RoadmapNode::NodeID> &parent) const
        -> bool {
        visited[current] = true;

        if (current == target) {
            return true;
        }

        for (auto next : nodes[current].next()) {
            if (!visited[next]) {
                parent[next] = current;
                if (dfs_impl(next, target, visited, parent)) {
                    return true;
                }
            }
        }
        return false;
    }

  public:
    static constexpr RoadmapNode::NodeID ROOT_ID = 0;
    explicit Roadmap(RoadmapInfo info) : _info(std::move(info)) {
        nodes.emplace_back(RoadmapNode{RoadmapNodeInfo{"Root", "Root node", {}}, {}, {}});
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

    [[nodiscard]] auto bfs_path(RoadmapNode::NodeID from, RoadmapNode::NodeID to) const
        -> std::optional<std::vector<RoadmapNode::NodeID>> {
        if (from >= size() || to >= size()) {
            return std::nullopt;
        }

        std::vector<bool> visited(size(), false);
        std::vector<RoadmapNode::NodeID> parent(size(),
                                                std::numeric_limits<RoadmapNode::NodeID>::max());
        std::queue<RoadmapNode::NodeID> q;

        visited[from] = true;
        q.push(from);

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current == to) {
                break;
            }

            for (auto next : nodes[current].next()) {
                if (!visited[next]) {
                    visited[next] = true;
                    parent[next] = current;
                    q.push(next);
                }
            }
        }

        if (!visited[to]) {
            return std::nullopt;
        }

        std::vector<RoadmapNode::NodeID> path;
        for (auto v = to; v != size(); v = parent[v]) {
            path.push_back(v);
            if (v == from) {
                break;
            }
        }

        std::ranges::reverse(path);
        return path;
    }

    [[nodiscard]] auto dfs_path(RoadmapNode::NodeID from, RoadmapNode::NodeID to) const
        -> std::optional<std::vector<RoadmapNode::NodeID>> {
        std::vector<bool> visited(size(), false);
        std::vector<RoadmapNode::NodeID> parent(size(),
                                                std::numeric_limits<RoadmapNode::NodeID>::max());

        bool found = dfs_impl(from, to, visited, parent);
        if (!found) {
            return std::nullopt;
        }

        std::vector<RoadmapNode::NodeID> path;
        for (auto v = to; v != size(); v = parent[v]) {
            path.push_back(v);
            if (v == from) {
                break;
            }
        }

        std::ranges::reverse(path);
        return path;
    }

    [[nodiscard]] auto reachable(RoadmapNode::NodeID from, RoadmapNode::NodeID to) const -> bool {
        std::vector<bool> visited(size(), false);
        std::vector<RoadmapNode::NodeID> parent(size(), size());
        return dfs_impl(from, to, visited, parent);
    }

    auto add_node(const RoadmapNodeInfo &info) -> RoadmapNode::NodeID {
        nodes.emplace_back(RoadmapNode{info, {}, {}});
        return nodes.size() - 1;
    }

    void add_edge(RoadmapNode::NodeID from, RoadmapNode::NodeID to) {
        if (from == to) {
            return;
        }
        if (to == ROOT_ID) {
            throw std::logic_error("Roadmap::add_edge: root node cannot be child");
        }
        if (from >= nodes.size() || to >= nodes.size()) {
            throw std::out_of_range("Roadmap::add_edge: node id out of range");
        }
        if (reachable(to, from)) {
            throw std::logic_error("Roadmap::add_edge: roadmap cannot be cyclic");
        }
        nodes[from].add_next(to);
        nodes[to].add_parent(from);
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
};
}; // namespace Lines
