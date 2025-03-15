#include "ecs.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

using f32 = float;

struct Type1 {
  f32 x, y;
  bool operator==(const Type1 &other) const {
    return (x == other.x && y == other.y);
  }
};

struct Type2 {
  f32 scale;
  f32 rotation;
  f32 translation;
  bool operator==(const Type2 &other) const {
    return (scale == other.scale && rotation == other.rotation &&
            translation == other.translation);
  }
};

TEST(ecs, init) {
  ECS ecs{};
  u32 e1 = ecs.CreateEntity();
  u32 e2 = ecs.CreateEntity();

  EXPECT_FALSE(e1 == e2);
}

TEST(ecs, has_component) {
  ECS ecs{};
  u32 e1 = ecs.CreateEntity();
  u32 e2 = ecs.CreateEntity();

  ecs.AddComponent(e1, Type1{22.0f, 0.0f});

  EXPECT_TRUE(ecs.HasComponent<Type1>(e1));
  EXPECT_FALSE(ecs.HasComponent<Type2>(e1));

  EXPECT_FALSE(ecs.HasComponent<Type1>(e2));
  EXPECT_FALSE(ecs.HasComponent<Type2>(e2));
}

// test you can get what you added, test you can't get on empty entity, test
// getting absent component fires assert
TEST(ecs, get_component) {
  ECS ecs{};
  u32 e1 = ecs.CreateEntity();
  Type1 t{22.0f, 0.0f};

  ecs.AddComponent(e1, t);

  EXPECT_EQ(ecs.GetComponent<Type1>(e1), t);
  EXPECT_DEBUG_DEATH(ecs.GetComponent<Type2>(e1), "failed");
  EXPECT_DEBUG_DEATH(ecs.GetComponent<Type1>(120), "failed");

  // Also try with rvalue
  ecs.AddComponent(e1, Type2{0.0f, 10.0f, 777.8f});
  auto t2 = Type2{0.0f, 10.0f, 777.8f};
  EXPECT_EQ(ecs.GetComponent<Type2>(e1), t2);
}

TEST(ecs, add_same_component) {
  ECS ecs{};
  u32 e1 = ecs.CreateEntity();
  Type1 t{22.0f, 0.0f};

  ecs.AddComponent(e1, t);
  EXPECT_DEBUG_DEATH(ecs.AddComponent(e1, t), "failed");
}

TEST(ecs, query_basic) {
  ECS ecs{};
  u32 e1 = ecs.CreateEntity();
  u32 e2 = ecs.CreateEntity();
  Type1 t{22.0f, 0.0f};

  // e1 has both types, e2 has only Type1
  ecs.AddComponent(e1, t);
  ecs.AddComponent(e1, Type2{0.0f, 10.0f, 777.8f});
  ecs.AddComponent(e2, Type1{0.3f, 60.8f});

  auto result1 = ecs.Query<Type1, Type2>(); // should contain e1
  auto result2 = ecs.Query<Type1>();        // should contain both e1 and e2

  EXPECT_EQ(result1.size(), 1);
  EXPECT_EQ(result2.size(), 2);

  // Check that result2 has both entities:
  EXPECT_NE(std::find_if(result2.begin(), result2.end(),
                         [e1](const auto &elem) { return elem == e1; }),
            result2.end());
  EXPECT_NE(std::find_if(result2.begin(), result2.end(),
                         [e2](const auto &elem) { return elem == e2; }),
            result2.end());

  // Check that result1 has only e1:
  EXPECT_NE(std::find_if(result1.begin(), result1.end(),
                         [e1](const auto &elem) { return elem == e1; }),
            result1.end());
  EXPECT_EQ(std::find_if(result1.begin(), result1.end(),
                         [e2](const auto &elem) { return elem == e2; }),
            result1.end());
}

TEST(ecs, query_no_result) {
  ECS ecs{};

  u32 e1 = ecs.CreateEntity();
  ecs.AddComponent(e1, Type1{4.0f, 28.5f});
  auto res = ecs.Query<Type2>();

  EXPECT_EQ(res.size(), 0);
}

TEST(ecs, remove_has_get) {
  ECS ecs{};
  u32 e1 = ecs.CreateEntity();
  u32 e2 = ecs.CreateEntity();
  Type1 t{22.0f, 0.0f};

  // e1 has both types, e2 has only Type1
  ecs.AddComponent(e1, t);
  ecs.AddComponent(e1, Type2{0.0f, 10.0f, 777.8f});
  ecs.AddComponent(e2, Type1{0.3f, 60.8f});

  ecs.RemoveComponent<Type2>(e1);

  // Check that component has been removed:
  EXPECT_FALSE(ecs.HasComponent<Type2>(e1));
  EXPECT_DEBUG_DEATH(ecs.GetComponent<Type2>(e1), "failed");

  // Check nothing else was touched:
  EXPECT_TRUE(ecs.HasComponent<Type1>(e1));
  EXPECT_TRUE(ecs.HasComponent<Type1>(e2));
}

TEST(ecs, remove_invalid) {
  ECS ecs{};
  u32 e1 = ecs.CreateEntity();
  u32 e2 = ecs.CreateEntity();

  ecs.AddComponent(e1, Type1{0.3f, 60.8f});

  // Remove absent component from existing entity:
  EXPECT_DEBUG_DEATH(ecs.RemoveComponent<Type1>(e2), "failed");
  EXPECT_DEBUG_DEATH(ecs.RemoveComponent<Type2>(e1), "failed");

  // Remove from non-existing entity:
  EXPECT_DEBUG_DEATH(ecs.RemoveComponent<Type2>(64), "failed");
}

TEST(ecs, remove_queries) {
  ECS ecs{};

  u32 e1 = ecs.CreateEntity();
  u32 e2 = ecs.CreateEntity();

  ecs.AddComponent(e1, Type1{0.9f, 20.1f});
  ecs.AddComponent(e1, Type2{0.0f, 10.0f, 777.8f});
  ecs.AddComponent(e2, Type1{0.3f, 60.8f});
  ecs.AddComponent(e2, Type2{1.0f, 101.3f, 747.2f});

  auto result1 = ecs.Query<Type1, Type2>(); // should contain both e1 and e2

  // Check that result1 has both entities:
  EXPECT_EQ(result1.size(), 2);
  EXPECT_NE(std::find_if(result1.begin(), result1.end(),
                         [e1](const auto &elem) { return elem == e1; }),
            result1.end());
  EXPECT_NE(std::find_if(result1.begin(), result1.end(),
                         [e2](const auto &elem) { return elem == e2; }),
            result1.end());

  // Remove a component from e2 and check that result has only e1:
  ecs.RemoveComponent<Type2>(e2);
  auto result2 = ecs.Query<Type1, Type2>();

  // Check that result2 has only e1:
  EXPECT_EQ(result2.size(), 1);
  EXPECT_NE(std::find_if(result2.begin(), result2.end(),
                         [e1](const auto &elem) { return elem == e1; }),
            result2.end());
  EXPECT_EQ(std::find_if(result2.begin(), result2.end(),
                         [e2](const auto &elem) { return elem == e2; }),
            result2.end());

  // Query only for Type1 and check that we get both e1 and e2:
  auto result3 = ecs.Query<Type1>();

  // Check that result3 has both entities:
  EXPECT_EQ(result3.size(), 2);
  EXPECT_NE(std::find_if(result3.begin(), result3.end(),
                         [e1](const auto &elem) { return elem == e1; }),
            result3.end());
  EXPECT_NE(std::find_if(result3.begin(), result3.end(),
                         [e2](const auto &elem) { return elem == e2; }),
            result3.end());
}

TEST(ecs, destroy_count_query) {
  ECS ecs{};
  auto e1 = ecs.CreateEntity();
  auto e2 = ecs.CreateEntity();

  ecs.AddComponent(e1, Type1{20.0f, 4.0f});
  ecs.AddComponent(e2, Type1{22.0f, 6.0f});
  auto result = ecs.Query<Type1>();

  EXPECT_EQ(ecs.Count(), 2);
  EXPECT_EQ(result.size(), 2);

  ecs.DestroyEntity(e2);
  auto result2 = ecs.Query<Type1>();

  EXPECT_EQ(ecs.Count(), 1);
  EXPECT_EQ(result2.size(), 1);
}

TEST(ecs, destroy_access) {
  ECS ecs{};
  auto e1 = ecs.CreateEntity();
  auto e2 = ecs.CreateEntity();

  ecs.AddComponent(e1, Type1{20.0f, 4.0f});
  ecs.AddComponent(e2, Type2{22.0f, 6.0f, 6534.2f});

  ecs.DestroyEntity(e2);

  EXPECT_DEBUG_DEATH(ecs.HasComponent<Type2>(e2), "failed");
  EXPECT_DEBUG_DEATH(ecs.GetComponent<Type2>(e2), "failed");
  EXPECT_DEBUG_DEATH(ecs.AddComponent(e2, Type1{0.8f, 5.0f}), "failed");
}

TEST(ecs, destroy_add_after) {
  ECS ecs{};
  auto e1 = ecs.CreateEntity();
  auto e2 = ecs.CreateEntity();
  auto t2 = Type2{22.0f, 6.0f, 6534.2f};

  ecs.AddComponent(e1, Type1{20.0f, 4.0f});
  ecs.AddComponent(e2, t2);

  EXPECT_EQ(ecs.Count(), 2);
  ecs.DestroyEntity(e2);
  EXPECT_EQ(ecs.Count(), 1);

  e2 = ecs.CreateEntity();
  ecs.AddComponent(e2, t2);

  EXPECT_EQ(ecs.Count(), 2);
  EXPECT_TRUE(ecs.HasComponent<Type2>(e2));
  EXPECT_EQ(ecs.GetComponent<Type2>(e2), t2);

  auto res = ecs.Query<Type2>();
  EXPECT_EQ(res.size(), 1);
}
