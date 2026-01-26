#include <gtest/gtest.h>
#include <lines/roadmaps.h>

using namespace Lines;

TEST(RoadmapNode, EmptyTitle) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    EXPECT_THROW(rmap.add_node(rmap.root(), RoadmapNodeInfo{""}), std::invalid_argument);
}

TEST(Roadmap, InfoAccessors) {
    Roadmap rmap{RoadmapInfo{"Rmap", "Desc", {"Tag1", "Tag2"}}};
    RoadmapNode *a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A", "Desc", {"Tag1", "Tag2"}});

    EXPECT_EQ(a->title(), "A");
    EXPECT_EQ(a->description().value(), "Desc");
    EXPECT_FALSE(a->tags().empty());

    // EXPECT_EQ(rmap.title(), "Rmap");
    // EXPECT_EQ(rmap.description().value(), "Desc");
    // EXPECT_FALSE(rmap.tags().empty());
}

TEST(Roadmap, LinearCreation) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    RoadmapNode *a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    RoadmapNode *b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    RoadmapNode *c = rmap.add_node(b, RoadmapNodeInfo{"C"});

    EXPECT_EQ(rmap[1], a);
    EXPECT_EQ(rmap[2], b);
    EXPECT_EQ(rmap[3], c);

    rmap.remove_node(c->id());

    EXPECT_NE(c, nullptr); // Now c is dangling pointer, so be careful
}

TEST(Roadmap, NonLinearCreation) {
    Roadmap rmap{RoadmapInfo{"Rmap"}};
    RoadmapNode *a = rmap.add_node(rmap.root(), RoadmapNodeInfo{"A"});
    RoadmapNode *b = rmap.add_node(a, RoadmapNodeInfo{"B"});
    RoadmapNode *c = rmap.add_node(b, RoadmapNodeInfo{"C"});
    RoadmapNode *d = rmap.add_node(b, RoadmapNodeInfo{"D"});

    EXPECT_EQ(rmap[1], a);
    EXPECT_EQ(rmap[2], b);
    EXPECT_EQ(rmap[3], c);
    EXPECT_EQ(rmap[4], d);

    auto bchildren = b->children();

    rmap.remove_node(b->id());

    EXPECT_EQ(bchildren, a->children()); // Children of b have been adopted by a after deletion
}
