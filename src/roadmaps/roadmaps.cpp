#include <lines/roadmaps/roadmaps.h>

#include <algorithm>
#include <stdexcept>

Lines::RoadmapNode::RoadmapNode(NodeID id, RoadmapNodeInfo info, NodePtr parent)
    : _id(id), _info(std::move(info)), _parent(std::move(parent)) {
    if (_info.title.empty()) {
        throw std::invalid_argument("RoadmapNode: title cannot be empty");
    }
}

auto Lines::RoadmapNode::title() const -> const std::string & { return _info.title; }

auto Lines::RoadmapNode::description() const -> const std::optional<std::string> & {
    return _info.description;
}

auto Lines::RoadmapNode::tags() const -> std::vector<std::string> { return _info.tags; }

auto Lines::RoadmapNode::state() const -> State { return _state; }

void Lines::RoadmapNode::set_state(State state) { _state = state; }

void Lines::RoadmapNode::add_child(const NodePtr &node) { _children.emplace_back(node); }

void Lines::RoadmapNode::remove_child(const NodePtr &child) {
    _children.erase(std::ranges::remove_if(
                        _children, [&](const NodePtr &el) { return el.lock() == child.lock(); })
                        .begin(),
                    _children.end());
}

void Lines::RoadmapNode::set_parent(NodePtr parent) { _parent = std::move(parent); }

auto Lines::RoadmapNode::out_degree() const -> std::size_t { return _children.size(); }

auto Lines::RoadmapNode::parent() const -> NodePtr { return _parent; }

auto Lines::RoadmapNode::children() const -> const std::vector<NodePtr> & { return _children; }

auto Lines::RoadmapNode::id() const -> NodeID { return _id; }

Lines::RoadmapNodeInfo::RoadmapNodeInfo(std::string title, std::optional<std::string> description,
                                        std::vector<std::string> tags)
    : title(std::move(title)), description(std::move(description)), tags(std::move(tags)) {}

Lines::RoadmapInfo::RoadmapInfo(std::string title, std::optional<std::string> description,
                                std::vector<std::string> tags)
    : title(std::move(title)), description(std::move(description)), tags(std::move(tags)) {}

auto Lines::Roadmap::free_id() -> RoadmapNode::NodeID {
    RoadmapNode::NodeID id = nodes.size();
    for (size_t i = 1; i < nodes.size(); ++i) {
        if (nodes[i] == nullptr) {
            return i;
        }
    }
    return id;
}

Lines::Roadmap::Roadmap(RoadmapInfo info) : _info(std::move(info)) {
    nodes.emplace_back(std::make_shared<RoadmapNode>(ROOT_ID, RoadmapNodeInfo{"Root", "Root node"},
                                                     RoadmapNode::NodePtr{}));
    if (_info.title.empty()) {
        throw std::invalid_argument("Roadmapinfo: title cannot be empty");
    }
}

auto Lines::Roadmap::operator[](RoadmapNode::NodeID id) -> RoadmapNode::NodePtr {
    return nodes[id];
}

auto Lines::Roadmap::root() -> RoadmapNode::NodePtr { return nodes[ROOT_ID]; }

auto Lines::Roadmap::root() const -> RoadmapNode::NodePtr { return nodes[ROOT_ID]; }

auto Lines::Roadmap::is_root(RoadmapNode::NodeID id) -> bool { return id == ROOT_ID; }

auto Lines::Roadmap::last() -> RoadmapNode::NodePtr { return nodes[nodes.size() - 1]; }

auto Lines::Roadmap::last() const -> RoadmapNode::NodePtr { return nodes[nodes.size() - 1]; }

auto Lines::Roadmap::last_id() const -> RoadmapNode::NodeID { return nodes.size() - 1; }

auto Lines::Roadmap::add_node(RoadmapNode::NodePtr parent, const RoadmapNodeInfo &info)
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

void Lines::Roadmap::remove_node(RoadmapNode::NodeID id) {
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

auto Lines::Roadmap::size() const -> std::size_t { return nodes.size(); }

auto Lines::Roadmap::title() const -> std::string { return _info.title; }

auto Lines::Roadmap::description() const -> std::optional<std::string> { return _info.description; }

auto Lines::Roadmap::tags() const -> std::vector<std::string> { return _info.tags; }
