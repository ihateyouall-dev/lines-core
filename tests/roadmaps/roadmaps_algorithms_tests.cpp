#include <gtest/gtest.h>
#include <lines/roadmaps/roadmaps_algorithms.hpp>

using namespace Lines;

TEST(RoadmapsAlgorithms, DFS) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    auto a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    auto b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    auto c = rmap.add_node(rmap.root(), RoadmapNodeInfo{"C"});

    std::vector<RoadmapNode::NodePtr> expected_dfs{rmap.root(), a, b, c};
    auto dfs = Roadmaps::dfs(rmap);

    ASSERT_EQ(dfs.size(), expected_dfs.size());

    for (int i = 0; i < expected_dfs.size(); ++i) {
        EXPECT_EQ(dfs[i].lock(), expected_dfs[i].lock());
    }
}

TEST(RoadmapsAlgorithms, BFS) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    auto a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    auto b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    auto c = rmap.add_node(rmap.root(), RoadmapNodeInfo{"C"});

    std::vector<RoadmapNode::NodePtr> expected_bfs{rmap.root(), a, c, b};
    auto bfs = Roadmaps::bfs(rmap);

    ASSERT_EQ(bfs.size(), expected_bfs.size());

    for (int i = 0; i < expected_bfs.size(); ++i) {
        EXPECT_EQ(bfs[i].lock(), expected_bfs[i].lock());
    }
}

TEST(RoadmapsAlgorithms, DFSForeach) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    auto a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    auto b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    auto c = rmap.add_node(rmap.root(), RoadmapNodeInfo{"C"});
    Roadmaps::dfs_foreach(
        rmap, [](const auto &node) { node.lock()->set_state(RoadmapNode::State::Completed); });
    for (const auto &node : Roadmaps::dfs(rmap)) {
        EXPECT_EQ(node.lock()->state(), RoadmapNode::State::Completed);
    }
}

TEST(RoadmapsAlgorithms, BFSForeach) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    auto a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    auto b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    auto c = rmap.add_node(rmap.root(), RoadmapNodeInfo{"C"});
    Roadmaps::bfs_foreach(
        rmap, [](const auto &node) { node.lock()->set_state(RoadmapNode::State::Completed); });
    for (const auto &node : Roadmaps::bfs(rmap)) {
        EXPECT_EQ(node.lock()->state(), RoadmapNode::State::Completed);
    }
}
