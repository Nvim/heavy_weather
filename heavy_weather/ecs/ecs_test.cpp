#include "ecs.hpp"
#include "heavy_weather/ecs/sparse.hpp"
#include "heavy_weather/engine.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
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

struct Uptr {
  UniquePtr<Type1> t1;

  explicit Uptr(UniquePtr<Type1> t1) : t1(std::move(t1)) {}
  Uptr(Uptr &&other) noexcept : t1(std::move(other.t1)) {}
  Uptr(const Uptr &) = delete;
  Uptr &operator=(const Uptr &) = delete;
  Uptr &operator=(Uptr &&other) noexcept {
    // Make sure we're not assigning to ourselves
    if (this != &other) {
      // Take resources from 'other' and make them ours
      this->t1 = std::move(other).t1;
    }
    return *this;
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
  EXPECT_DEBUG_DEATH(ecs.GetComponent<Type2>(e1), "");
  EXPECT_DEBUG_DEATH(ecs.GetComponent<Type1>(120), "");

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
  EXPECT_DEBUG_DEATH(ecs.AddComponent(e1, t), "");
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
  EXPECT_DEBUG_DEATH(ecs.GetComponent<Type2>(e1), "");

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
  EXPECT_DEBUG_DEATH(ecs.RemoveComponent<Type1>(e2), "");
  EXPECT_DEBUG_DEATH(ecs.RemoveComponent<Type2>(e1), "");

  // Remove from non-existing entity:
  EXPECT_DEBUG_DEATH(ecs.RemoveComponent<Type2>(64), "");
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

TEST(ecs, destroy_complex) {
  ECS ecs{};
  auto e1 = ecs.CreateEntity();
  auto e2 = ecs.CreateEntity();
  auto e3 = ecs.CreateEntity();

  ecs.AddComponent(e1, Type1{20.0f, 4.0f});
  ecs.AddComponent(e2, Type1{22.0f, 6.0f});
  ecs.AddComponent(e3, Type1{24.0f, 8.0f});

  ecs.AddComponent(e1, Type2{20.0f, 4.0f, 3.0f});
  ecs.AddComponent(e2, Type2{22.0f, 6.0f, 8.8f});
  ecs.AddComponent(e3, Type2{22.0f, 6.0f, 67.3f});

  auto result = ecs.Query<Type1, Type2>();
  EXPECT_EQ(result.size(), 3);

  ecs.DestroyEntity(e1);
  auto result2 = ecs.Query<Type1, Type2>();

  EXPECT_EQ(ecs.Count(), 2);
  EXPECT_EQ(result2.size(), 2);
  EXPECT_GT(result2[0], e1);
  EXPECT_GT(result2[1], e1);
  // auto expect = Type1{22.0f, 6.0f};
  EXPECT_NE(ecs.GetComponentPtr<Type1>(result2[0])->x, 20.0f);
  EXPECT_NE(ecs.GetComponentPtr<Type1>(result2[0])->y, 4.0f);
  EXPECT_NE(ecs.GetComponentPtr<Type1>(result2[1])->x, 20.0f);
  EXPECT_NE(ecs.GetComponentPtr<Type1>(result2[1])->y, 4.0f);
}

TEST(ecs, destroy_access) {
  ECS ecs{};
  auto e1 = ecs.CreateEntity();
  auto e2 = ecs.CreateEntity();

  ecs.AddComponent(e1, Type1{20.0f, 4.0f});
  ecs.AddComponent(e2, Type2{22.0f, 6.0f, 6534.2f});

  ecs.DestroyEntity(e2);

  EXPECT_DEBUG_DEATH(ecs.HasComponent<Type2>(e2), "");
  EXPECT_DEBUG_DEATH(ecs.GetComponent<Type2>(e2), "");
  EXPECT_DEBUG_DEATH(ecs.AddComponent(e2, Type1{0.8f, 5.0f}), "");
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

TEST(ecs, move_semantics) {
  ECS ecs{};
  auto e1 = ecs.CreateEntity();
  auto e2 = ecs.CreateEntity();
  auto e3 = ecs.CreateEntity();

  ecs.AddComponent(e1, Uptr{std::make_unique<Type1>(Type1{1.0f, 2.0f})});
  ecs.AddComponent(e2, Uptr{std::make_unique<Type1>(Type1{3.0f, 4.0f})});
  ecs.AddComponent(e3, Uptr{std::make_unique<Type1>(Type1{5.0f, 6.0f})});

  EXPECT_EQ(ecs.GetComponent<Uptr>(e1).t1->x, 1.0f);
  EXPECT_EQ(ecs.GetComponent<Uptr>(e1).t1->y, 2.0f);
  EXPECT_EQ(ecs.GetComponent<Uptr>(e2).t1->x, 3.0f);
  EXPECT_EQ(ecs.GetComponent<Uptr>(e2).t1->y, 4.0f);
  EXPECT_EQ(ecs.GetComponent<Uptr>(e3).t1->x, 5.0f);
  EXPECT_EQ(ecs.GetComponent<Uptr>(e3).t1->y, 6.0f);

  ecs.DestroyEntity(e1);

  // Check that query result is good:
  auto result = ecs.Query<Uptr>();
  EXPECT_EQ(result.size(), 2);
  EXPECT_NE(result.at(0), e1);
  EXPECT_NE(result.at(1), e1);

  // Check both GetComponent variants:
  EXPECT_TRUE(ecs.HasComponent<Uptr>(e2));
  EXPECT_TRUE(ecs.HasComponent<Uptr>(e3));
  EXPECT_EQ(ecs.GetComponent<Uptr>(e2).t1->x, 3.0f);
  EXPECT_EQ(ecs.GetComponent<Uptr>(e2).t1->y, 4.0f);
  EXPECT_EQ(ecs.GetComponent<Uptr>(e3).t1->x, 5.0f);
  EXPECT_EQ(ecs.GetComponent<Uptr>(e3).t1->y, 6.0f);
  EXPECT_EQ(ecs.GetComponentPtr<Uptr>(e2)->t1->x, 3.0f);
  EXPECT_EQ(ecs.GetComponentPtr<Uptr>(e2)->t1->y, 4.0f);
  EXPECT_EQ(ecs.GetComponentPtr<Uptr>(e3)->t1->x, 5.0f);
  EXPECT_EQ(ecs.GetComponentPtr<Uptr>(e3)->t1->y, 6.0f);

  // Check internals as well:
  const auto &pool_map = ecs.Pools();
  EXPECT_EQ(pool_map.size(), 1);
  auto pool = pool_map.find(TYPE_IDX(Uptr));
  EXPECT_NE(pool, pool_map.end());
  ISparseSet &s = *pool->second;
  auto &set = dynamic_cast<SparseSet<Uptr> &>(s);

  EXPECT_EQ(set.IDs().size(), 2);
  EXPECT_EQ(set.Values().size(), 2);
  // e3:
  EXPECT_EQ(set.IDs().at(0), e3);
  EXPECT_EQ(set.Values().at(0).t1->x, 5.0f);
  EXPECT_EQ(set.Values().at(0).t1->y, 6.0f);
  EXPECT_EQ(set.Get(e3).t1->x, 5.0f);
  EXPECT_EQ(set.Get(e3).t1->y, 6.0f);

  // e2:
  EXPECT_EQ(set.IDs().at(1), e2);
  EXPECT_EQ(set.Values().at(1).t1->x, 3.0f);
  EXPECT_EQ(set.Values().at(1).t1->y, 4.0f);
  EXPECT_EQ(set.Get(e2).t1->x, 3.0f);
  EXPECT_EQ(set.Get(e2).t1->y, 4.0f);
}
