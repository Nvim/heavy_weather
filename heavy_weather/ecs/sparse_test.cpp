#include "sparse.hpp"
#include <gtest/gtest.h>
#include <string>

using f32 = float;

struct SomeType {
  int a;
  float b;
  std::string str;

  bool operator ==(const SomeType& other) const {
    return (a == other.a && b == other.b && str == other.str);
  }
};

TEST(primitive, init) {
  SparseSet<f32> s{};
  EXPECT_EQ(s.Count(), 0);
}

TEST(primitive, add) {
  SparseSet<f32> s{};
  s.Add(42, 1.0f);
  EXPECT_EQ(s.Count(), 1);
  EXPECT_TRUE(s.Has(42));
  EXPECT_FALSE(s.Has(41));

  s.Add(41, 44.0f);
  EXPECT_EQ(s.Count(), 2);
  EXPECT_TRUE(s.Has(41));

  // Add already present element:
  EXPECT_DEBUG_DEATH(s.Add(42, 33.0f), "failed");
}

TEST(primitive, has) {
  SparseSet<f32> s{};
  EXPECT_FALSE(s.Has(2));

  // inbounds check:
  for(int i = 0; i < kSparseBaseLen; ++i) {
    EXPECT_FALSE(s.Has(i));
  }

  // oob:
  EXPECT_FALSE(s.Has(kMaxId - 2));

  s.Add(2, 24.0f);
  EXPECT_TRUE(s.Has(2));
}

TEST(primitive, get) {
  SparseSet<unsigned long> s{};

  // Test with rvalue litteral:
  s.Add(5, 400UL);
  EXPECT_EQ(s.Get(5), 400UL);

  // Test with lvalue:
  unsigned long l = 2000UL;
  s.Add(40, l);
  EXPECT_EQ(s.Get(40), l);

  // Get invalid index:
  EXPECT_DEBUG_DEATH(s.Get(505), "failed");
}

TEST(primitive, remove_has) {
  SparseSet<u32> s{};
  s.Add(1, 800);
  s.Add(8, 4);

  s.Remove(8);
  EXPECT_FALSE(s.Has(8));
  EXPECT_TRUE(s.Has(1));

  s.Remove(1);
  EXPECT_FALSE(s.Has(1));
}

TEST(primitive, remove_get) {
  SparseSet<u32> s{};
  s.Add(1, 800);
  s.Add(8, 4);

  s.Remove(8);
  EXPECT_DEBUG_DEATH(s.Get(8), "failed");

  EXPECT_EQ(800, s.Get(1));
  s.Remove(1);
  EXPECT_DEBUG_DEATH(s.Get(1), "failed");
}

TEST(primitive, remove_count) {
  SparseSet<u32> s{};
  s.Add(1, 800);
  s.Add(8, 4);

  s.Remove(8);
  EXPECT_EQ(s.Count(), 1);
  s.Remove(1);
  EXPECT_EQ(s.Count(), 0);
}

TEST(custom_type, add_get_has) {
  SparseSet<SomeType> s{};
  SomeType a{11, 12.0f, "a"};
  SomeType b{19, 42.2f, "b"};
  SomeType c{0, 55.5f, "c"};

  s.Add(1, a);
  s.Add(2, b);
  s.Add(3, {0, 55.5f, "c"}); // Test for rvalue too

  EXPECT_EQ(s.Count(), 3);

  EXPECT_TRUE(s.Has(1));
  EXPECT_TRUE(s.Has(2));
  EXPECT_TRUE(s.Has(3));
  for (int i = 4; i < 1000; ++i) {
    EXPECT_FALSE(s.Has(i));
  }

  SomeType expect = SomeType{11, 12.0f, "a"};
  EXPECT_EQ(expect, s.Get(1));
  expect = SomeType{19, 42.2f, "b"};
  EXPECT_EQ(expect, s.Get(2));
  expect = SomeType{0, 55.5f, "c"};
  EXPECT_EQ(expect, s.Get(3));
}

TEST(custom_type, remove) {
  SparseSet<SomeType> s{};
  SomeType a{11, 12.0f, "a"};
  SomeType b{19, 42.2f, "b"};
  s.Add(1, a);
  s.Add(2, b);

  s.Remove(2);
  EXPECT_FALSE(s.Has(2));
  EXPECT_EQ(s.Count(), 1);
  
  s.Remove(1);
  EXPECT_FALSE(s.Has(1));
  EXPECT_EQ(s.Count(), 0);
}
