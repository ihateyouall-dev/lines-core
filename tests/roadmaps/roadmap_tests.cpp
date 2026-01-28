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

    EXPECT_EQ(rmap.title(), "Rmap");
    EXPECT_EQ(rmap.description().value(), "Desc");
    EXPECT_FALSE(rmap.tags().empty());
}

TEST(Roadmap, LinearCreation) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    RoadmapNode::NodePtr a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    RoadmapNode::NodePtr b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    RoadmapNode::NodePtr c = rmap.add_node(b, RoadmapNodeInfo{"C"});

    EXPECT_EQ(rmap[1].lock(), a.lock());
    EXPECT_EQ(rmap[2].lock(), b.lock());
    EXPECT_EQ(rmap[3].lock(), c.lock());

    rmap.remove_node(c.lock()->id());

    EXPECT_TRUE(c.expired());
}

TEST(Roadmap, NonLinearCreation) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    RoadmapNode::NodePtr a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    RoadmapNode::NodePtr b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    RoadmapNode::NodePtr c = rmap.add_node(b, RoadmapNodeInfo{"C"});
    RoadmapNode::NodePtr d = rmap.add_node(b, RoadmapNodeInfo{"D"});

    EXPECT_EQ(rmap[1].lock(), a.lock());
    EXPECT_EQ(rmap[2].lock(), b.lock());
    EXPECT_EQ(rmap[3].lock(), c.lock());
    EXPECT_EQ(rmap[4].lock(), d.lock());

    auto bchildren = b.lock()->children();

    rmap.remove_node(b.lock()->id());
}

TEST(Roadmap, Accessors) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    RoadmapNode::NodePtr a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    RoadmapNode::NodePtr b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    RoadmapNode::NodePtr c = rmap.add_node(a, RoadmapNodeInfo{"C"});

    EXPECT_EQ(rmap.size(), 4); // FIXME
    EXPECT_EQ(rmap.last().lock(), c.lock());
    EXPECT_EQ(rmap.last_id(), 3);
    EXPECT_TRUE(rmap.is_root(Roadmap::ROOT_ID));
}

TEST(RoadmapNode, States) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    RoadmapNode::NodePtr a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    EXPECT_EQ(a.lock()->state(), RoadmapNode::State::NotCompleted);

    RoadmapNode::State state = RoadmapNode::State::InProgress;

    a.lock()->set_state(state);

    EXPECT_EQ(a.lock()->state(), state);

    state = RoadmapNode::State::Skipped;

    a.lock()->set_state(state);

    EXPECT_EQ(a.lock()->state(), state);

    state = RoadmapNode::State::Completed;

    a.lock()->set_state(state);

    EXPECT_EQ(a.lock()->state(), state);
}

TEST(RoadmapNode, Accessors) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    RoadmapNode::NodePtr a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    RoadmapNode::NodePtr b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    RoadmapNode::NodePtr c = rmap.add_node(a, RoadmapNodeInfo{"C"});

    EXPECT_EQ(a.lock()->out_degree(), 2);

    EXPECT_EQ(b.lock()->parent().lock(), a.lock());
    EXPECT_EQ(b.lock()->parent().lock(), c.lock()->parent().lock());

    EXPECT_EQ(a.lock()->id(), 1);
    EXPECT_EQ(b.lock()->id(), 2);
    EXPECT_EQ(c.lock()->id(), 3);

    EXPECT_EQ(a.lock()->children()[0].lock(), b.lock());
    EXPECT_EQ(a.lock()->children()[1].lock(), c.lock());
}
