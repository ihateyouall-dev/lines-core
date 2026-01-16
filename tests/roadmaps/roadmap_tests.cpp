#include <gtest/gtest.h>
#include <lines/roadmaps.h>

using namespace Lines;

TEST(RoadmapInvariants, DAG) {
    Roadmap roadmap{RoadmapInfo{"Rmap"}};

    RoadmapNode::NodeID a = roadmap.add_node(RoadmapNodeInfo{"A"});
    RoadmapNode::NodeID b = roadmap.add_node(RoadmapNodeInfo{"B"});
    RoadmapNode::NodeID c = roadmap.add_node(RoadmapNodeInfo{"C"});
    RoadmapNode::NodeID d = roadmap.add_node(RoadmapNodeInfo{"D"});

    roadmap.add_edge(a, b);
    roadmap.add_edge(b, c);
    roadmap.add_edge(c, d);

    EXPECT_THROW(roadmap.add_edge(d, a),
                 std::logic_error); // DAG invariant: cycles are forbidden

    EXPECT_NO_THROW(roadmap.add_edge(a, a)); // Just nothing happens
    EXPECT_THROW(roadmap.add_edge(999, 1000), std::out_of_range);
    EXPECT_THROW(roadmap.add_edge(roadmap.size(), Roadmap::ROOT_ID),
                 std::logic_error); // Root node cannot be child
}

TEST(RoadmapPaths, DFS) {
    Roadmap roadmap{RoadmapInfo{"Rmap"}};

    RoadmapNode::NodeID a = roadmap.add_node(RoadmapNodeInfo{"A"});
    RoadmapNode::NodeID b = roadmap.add_node(RoadmapNodeInfo{"B"});
    RoadmapNode::NodeID c = roadmap.add_node(RoadmapNodeInfo{"C"});
    RoadmapNode::NodeID d = roadmap.add_node(RoadmapNodeInfo{"D"});

    roadmap.add_edge(a, b);
    roadmap.add_edge(b, c);
    roadmap.add_edge(c, d);

    EXPECT_TRUE(roadmap.reachable(a, b));
    EXPECT_TRUE(roadmap.reachable(b, c));
    EXPECT_TRUE(roadmap.reachable(c, d));
    EXPECT_FALSE(roadmap.reachable(d, a));

    RoadmapNode::NodeID e = roadmap.add_node(RoadmapNodeInfo{"E"});
    roadmap.add_edge(a, e);
    EXPECT_TRUE(roadmap.reachable(a, e));

    std::vector<RoadmapNode::NodeID> path = roadmap.dfs_path(a, e).value();
    EXPECT_EQ(path, std::vector<RoadmapNode::NodeID>({a, e}));
}

TEST(RoadmapPaths, BFS) {
    Roadmap roadmap{RoadmapInfo{"Rmap"}};

    RoadmapNode::NodeID a = roadmap.add_node(RoadmapNodeInfo{"A"});
    RoadmapNode::NodeID b = roadmap.add_node(RoadmapNodeInfo{"B"});
    RoadmapNode::NodeID c = roadmap.add_node(RoadmapNodeInfo{"C"});
    RoadmapNode::NodeID d = roadmap.add_node(RoadmapNodeInfo{"D"});

    roadmap.add_edge(a, b);
    roadmap.add_edge(b, c);
    roadmap.add_edge(c, d);

    std::vector<RoadmapNode::NodeID> path = roadmap.bfs_path(a, d).value();
    EXPECT_EQ(path, std::vector<RoadmapNode::NodeID>({a, b, c, d}));
}

TEST(RoadmapPaths, UnreachablePaths) {
    Roadmap roadmap{RoadmapInfo{"Rmap"}};

    RoadmapNode::NodeID a = roadmap.add_node(RoadmapNodeInfo{"A"});
    RoadmapNode::NodeID b = roadmap.add_node(RoadmapNodeInfo{"B"});

    EXPECT_FALSE(roadmap.reachable(a, b));
    EXPECT_EQ(roadmap.dfs_path(a, b), std::nullopt);
    EXPECT_EQ(roadmap.bfs_path(a, b), std::nullopt);
    EXPECT_EQ(roadmap.dfs_path(roadmap.size() + 1, roadmap.size() + 2), std::nullopt);
    EXPECT_EQ(roadmap.bfs_path(roadmap.size() + 1, roadmap.size() + 2), std::nullopt);
}
