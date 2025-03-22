#pragma once
#include <limits>
#include <utility>
#include <vector>

#include <heavy_weather/core/Asserts.hpp>
#include <heavy_weather/engine.h>

// Inital element count of the vector, to avoid reallocs.
constexpr u32 kSparseBaseLen = 512;
constexpr u32 kMaxId = std::numeric_limits<unsigned int>::max();

// 0 is reserved to designate an absent value
constexpr u32 kEmpty = kMaxId;

class ISparseSet {
public:
  virtual u32 Count() const = 0;
  virtual bool Has(u32 index) const = 0;
  virtual const std::vector<u32> &GetAllIds() const = 0;
  virtual void Remove(u32 index) = 0;
  // virtual T Get(u32 index) const = 0;
  // virtual void Add(u32 index, T &value) = 0;
  // virtual void Add(u32 index, T &&value) = 0;
  //
  virtual ~ISparseSet() = default;
  ISparseSet() = default;
  ISparseSet(const ISparseSet &) = default;
  ISparseSet(ISparseSet &&) = delete;
  ISparseSet &operator=(const ISparseSet &) = default;
  ISparseSet &operator=(ISparseSet &&) = delete;
};

// SparseSet owns the component instances (T's)
template <typename T> class SparseSet : public ISparseSet {
public:
  explicit SparseSet() : sparse_(std::vector<u32>{kSparseBaseLen}) {
    sparse_.assign(kSparseBaseLen, kEmpty);
  }

  u32 Count() const override {
    return count_;
  }

  bool Has(u32 index) const override {
    if (index >= sparse_.size()) {
      return false;
    }
    return ((index < sparse_.size()) && (sparse_[index] != kEmpty));
  }

  T &Get(u32 index) {
    HW_ASSERT(count_ == packed_ids_.size());
    HW_ASSERT(Has(index));
    return packed_values_[sparse_[index]];
  }

  T *GetPtr(u32 index) {
    HW_ASSERT(count_ == packed_ids_.size());
    HW_ASSERT(Has(index));
    return &packed_values_[sparse_[index]];
  }

  const std::vector<u32> &GetAllIds() const override {
    HW_ASSERT(count_ == packed_ids_.size());
    return packed_ids_;
  }

  const std::vector<T &> &GetAllComponents() const { return packed_values_; }

  /* Add a new entity to the set. Modifying the component of an existing entity
   * is not supported. */
  void Add(u32 index, T &value) {
    HW_ASSERT(!Has(index));
    packed_ids_.push_back(index);
    packed_values_.push_back(value);
    sparse_[index] = count_++;
  }

  void Add(u32 index, T &&value) {
    HW_ASSERT(!Has(index));
    packed_ids_.push_back(index);
    packed_values_.push_back(std::move(value));
    sparse_[index] = count_++;
  }

  /* Shrink the packed arrays, remove link in spare.
   * Removing an absent entity will trigger an assert in debug */
  void Remove(u32 index) override {
    HW_ASSERT(Has(index));

    u32 last_sparse_idx = packed_ids_[count_-1];

    std::swap(packed_ids_[sparse_[index]], packed_ids_.back());
    packed_ids_.pop_back();

    // std::swap(packed_values_[sparse_[index]], packed_values_.back());
    packed_values_[sparse_[index]] = T(std::move(packed_values_.back()));
    packed_values_.pop_back();

    --count_;
    sparse_[last_sparse_idx] = sparse_[index];
    sparse_[index] = kEmpty;
    HW_ASSERT(count_ == packed_ids_.size());
  }

  std::vector<T>& Values() {return packed_values_; }
  std::vector<u32>& IDs() { return packed_ids_; }

private:
  u32 count_{};
  std::vector<u32> packed_ids_;    // entity ids
  std::vector<T> packed_values_{}; // components
  std::vector<u32> sparse_;        // external array
};

/* Simplest implementation, only one packed array for storing IDs.
This is meant to be used by ECS for storing all IDs. */
class BaseSparseSet : public ISparseSet {
private:
  u32 count_{};
  std::vector<u32> packed_ids_; // entity ids
  std::vector<u32> sparse_;     // external array

public:
  explicit BaseSparseSet() : sparse_(std::vector<u32>{kSparseBaseLen}) {
    sparse_.assign(kSparseBaseLen, kEmpty);
  }

  u32 Count() const override { return count_; }

  bool Has(u32 index) const override { return sparse_[index] != kEmpty; }

  /* Add a new entity to the set. Modifying the component of an existing entity
   * is not supported. */
  void Add(u32 index) {
    HW_ASSERT(!Has(index));
    packed_ids_.push_back(index);
    sparse_[index] = count_++;
  }

  /* Shrink the packed arrays, remove link in spare.
   * Removing an absent entity will trigger an HW_ASSERT in debug */
  void Remove(u32 index) override {
    HW_ASSERT(Has(index));

    std::swap(packed_ids_[sparse_[index]], packed_ids_.back());
    packed_ids_.pop_back();

    sparse_[index] = kEmpty;
    count_--;
  }

  const std::vector<u32> &GetAllIds() const override { return packed_ids_; }
};
