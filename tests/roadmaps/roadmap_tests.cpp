#include <gtest/gtest.h>
#include <lines/roadmaps.h>

using namespace Lines;

TEST(RoadmapNode, EmptyTitle) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    EXPECT_THROW(rmap.add_node(rmap.root(), RoadmapNodeInfo{""}), std::invalid_argument);
}

TEST(Roadmap, InfoAccessors) {
    Roadmap rmap{RoadmapInfo{"Rmap", "Desc", {"Tag1", "Tag2"}}};
    RoadmapNode::NodePtr a =
        rmap.add_node(rmap.root(), RoadmapNodeInfo{"A", "Desc", {"Tag1", "Tag2"}});

    EXPECT_EQ(a.lock()->title(), "A");
    EXPECT_EQ(a.lock()->description().value(), "Desc");
    EXPECT_FALSE(a.lock()->tags().empty());

    // EXPECT_EQ(rmap.title(), "Rmap");
    // EXPECT_EQ(rmap.description().value(), "Desc");
    // EXPECT_FALSE(rmap.tags().empty());
}

TEST(Roadmap, LinearCreation) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    RoadmapNode::NodePtr a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    RoadmapNode::NodePtr b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    RoadmapNode::NodePtr c = rmap.add_node(b, RoadmapNodeInfo{"C"});

    EXPECT_EQ(rmap[1], a);
    EXPECT_EQ(rmap[2], b);
    EXPECT_EQ(rmap[3], c);

    rmap.remove_node(c.lock()->id());

    EXPECT_TRUE(c.expired());
}

TEST(Roadmap, NonLinearCreation) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    RoadmapNode::NodePtr a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    RoadmapNode::NodePtr b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    RoadmapNode::NodePtr c = rmap.add_node(b, RoadmapNodeInfo{"C"});
    RoadmapNode::NodePtr d = rmap.add_node(b, RoadmapNodeInfo{"D"});

    EXPECT_EQ(rmap[1], a);
    EXPECT_EQ(rmap[2], b);
    EXPECT_EQ(rmap[3], c);
    EXPECT_EQ(rmap[4], d);

    auto bchildren = b.lock()->children();

    rmap.remove_node(b.lock()->id());

    EXPECT_EQ(bchildren,
              a.lock()->children()); // Children of b have been adopted by a after deletion
}
