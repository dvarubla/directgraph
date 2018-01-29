#include <gtest/gtest.h>
#include <PixelContainer.h>

using namespace directgraph;

namespace {
    class PixelContainerTest : public ::testing::Test {
    public:
        typedef PixelContainer<ColorFormat::R8G8B8> PCont;
        struct PContParams{
            uint_fast32_t left;
            uint_fast32_t top;
            uint_fast32_t right;
            uint_fast32_t bottom;
            uint_fast32_t stride;
            uint_fast32_t lastStride;
            uint_fast32_t height;
        };
    protected:
        PixelContainerTest() {
        }

        virtual ~PixelContainerTest() {
        }

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
        void compareContainers(PCont &l, PContParams &r){
            Rectangle firstCoords = l.getFirstCoords();
            Rectangle lastCoords = l.getLastCoords();
            EXPECT_EQ(firstCoords.left, r.left);
            EXPECT_EQ(firstCoords.right, r.right);
            EXPECT_EQ(std::min(firstCoords.top, lastCoords.top), r.top);
            EXPECT_EQ(std::max(firstCoords.bottom, lastCoords.bottom), r.bottom);
            EXPECT_EQ(l.getStride(), r.stride);
            EXPECT_EQ(l.getLastStride(), r.lastStride);
            EXPECT_EQ(l.getHeight(), r.height);
        }
    };
}

static const uint32_t WIDTH = 200;
static const uint32_t HEIGHT = 300;

TEST_F(PixelContainerTest, AddTrueSimple){
    PCont pc(100, 100, 0, 101, 100, 0, WIDTH, HEIGHT);
    EXPECT_TRUE(pc.tryAddPixel(102, 100, 0));
    PContParams params = {100, 100, 102, 100, 3 * sizeof(uint32_t), 3 * sizeof(uint32_t), 1};
    compareContainers(pc, params);
}

TEST_F(PixelContainerTest, AddTrueVerticalSimple){
    PCont pc(100, 100, 0, 100, 101, 0, WIDTH, HEIGHT);
    EXPECT_TRUE(pc.tryAddPixel(100, 102, 0));
    PContParams params = {100, 100, 100, 102, sizeof(uint32_t), sizeof(uint32_t), 3};
    compareContainers(pc, params);
}

TEST_F(PixelContainerTest, AddTrueVerticalTopDown){
    PCont pc(100, 100, 0, 100, 101, 0, WIDTH, HEIGHT);
    EXPECT_TRUE(pc.tryAddPixel(100, 102, 0));
    pc.tryAddPixel(100, 103, 0);
    pc.tryAddPixel(100, 104, 0);
    pc.tryAddPixel(100, 105, 0);
    pc.tryAddPixel(100, 106, 0);
    PContParams params = {100, 100, 100, 106, sizeof(uint32_t), sizeof(uint32_t), 7};
    compareContainers(pc, params);
}

TEST_F(PixelContainerTest, AddTrueVerticalBottomUp){
    PCont pc(100, 100, 0, 100, 99, 0, WIDTH, HEIGHT);
    EXPECT_TRUE(pc.tryAddPixel(100, 98, 0));
    pc.tryAddPixel(100, 97, 0);
    pc.tryAddPixel(100, 96, 0);
    pc.tryAddPixel(100, 95, 0);
    pc.tryAddPixel(100, 94, 0);
    PContParams params = {100, 94, 100, 100, sizeof(uint32_t), sizeof(uint32_t), 7};
    compareContainers(pc, params);
}

TEST_F(PixelContainerTest, AddFalseSimple){
    PCont pc(100, 100, 0, 101, 100, 0, WIDTH, HEIGHT);
    EXPECT_FALSE(pc.tryAddPixel(103, 100, 0));
    EXPECT_FALSE(pc.tryAddPixel(101, 102, 0));
    PContParams params = {100, 100, 101, 100, 2 * sizeof(uint32_t), 2 * sizeof(uint32_t), 1};
    compareContainers(pc, params);
}

TEST_F(PixelContainerTest, AddTrueOneLine){
    PCont pc(100, 100, 0, 101, 100, 0, WIDTH, HEIGHT);
    pc.tryAddPixel(102, 100, 0);
    pc.tryAddPixel(103, 100, 0);
    pc.tryAddPixel(104, 100, 0);
    pc.tryAddPixel(105, 100, 0);
    EXPECT_TRUE(pc.tryAddPixel(106, 100, 0));
    PContParams params = {100, 100, 106, 100, 7 * sizeof(uint32_t), 7 * sizeof(uint32_t), 1};
    compareContainers(pc, params);
}

TEST_F(PixelContainerTest, AddTrueManyLinesTopDown){
    PCont pc(100, 100, 0, 101, 100, 0, WIDTH, HEIGHT);
    pc.tryAddPixel(102, 100, 0);
    pc.tryAddPixel(103, 100, 0);
    pc.tryAddPixel(104, 100, 0);
    pc.tryAddPixel(105, 100, 0);

    EXPECT_TRUE(pc.tryAddPixel(100, 101, 0));
    pc.tryAddPixel(101, 101, 0);
    pc.tryAddPixel(102, 101, 0);
    pc.tryAddPixel(103, 101, 0);
    pc.tryAddPixel(104, 101, 0);
    pc.tryAddPixel(105, 101, 0);

    EXPECT_TRUE(pc.tryAddPixel(100, 102, 0));
    pc.tryAddPixel(101, 102, 0);
    pc.tryAddPixel(102, 102, 0);
    pc.tryAddPixel(103, 102, 0);
    pc.tryAddPixel(104, 102, 0);
    pc.tryAddPixel(105, 102, 0);

    PContParams params = {100, 100, 105, 102, 6 * sizeof(uint32_t), 6 * sizeof(uint32_t), 3};
    compareContainers(pc, params);
}

TEST_F(PixelContainerTest, AddTrueManyLinesBottomUp){
    PCont pc(100, 100, 0, 101, 100, 0, WIDTH, HEIGHT);
    pc.tryAddPixel(102, 100, 0);
    pc.tryAddPixel(103, 100, 0);
    pc.tryAddPixel(104, 100, 0);
    pc.tryAddPixel(105, 100, 0);

    EXPECT_TRUE(pc.tryAddPixel(100, 99, 0));
    pc.tryAddPixel(101, 99, 0);
    pc.tryAddPixel(102, 99, 0);
    pc.tryAddPixel(103, 99, 0);
    pc.tryAddPixel(104, 99, 0);
    pc.tryAddPixel(105, 99, 0);

    EXPECT_TRUE(pc.tryAddPixel(100, 98, 0));
    pc.tryAddPixel(101, 98, 0);
    pc.tryAddPixel(102, 98, 0);
    pc.tryAddPixel(103, 98, 0);
    pc.tryAddPixel(104, 98, 0);
    pc.tryAddPixel(105, 98, 0);

    PContParams params = {100, 98, 105, 100, 6 * sizeof(uint32_t), 6 * sizeof(uint32_t), 3};
    compareContainers(pc, params);
}