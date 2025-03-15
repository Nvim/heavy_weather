#pragma once

#include "sparse.hpp"
#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>

#define TYPE_IDX(T) std::type_index(typeid(T))

enum class ComponentType : char {
  COMP_TRANSFORM,
  COMP_DIRTY,
  COMP_HIERARCHY,
  COMP_VERTICES,
};

class ECS {

private:
  BaseSparseSet entities_;
  std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>> pools_;

public:
  u32 CreateEntity() {
    // TODO: recycle at some point
    assert(entities_.Count() < kMaxId - 1);
    u32 id = NewId();
    entities_.Add(id);
    return id;
  }

  u32 Count() const { return entities_.Count(); }

  void DestroyEntity(u32 entity) {
    // TODO: recycle at some point
    assert(entities_.Has(entity));
    for (auto &p : pools_) {
      if (p.second->Has(entity)) {
        p.second->Remove(entity);
      }
    }
    entities_.Remove(entity);
  }

  template <typename T>
  void AddComponent(u32 entity, T &component) {
    assert(entities_.Has(entity));
    auto &p = GetOrCreatePool<T>();
    p.Add(entity, component);
  }

  template <typename T>
  void AddComponent(u32 entity, T &&component) {
    assert(entities_.Has(entity));
    auto &p = GetOrCreatePool<T>();
    p.Add(entity, std::forward<T>(component));
  }

  template <typename T>
  bool HasComponent(u32 entity) const {
    assert(entities_.Has(entity));
    auto p = pools_.find(TYPE_IDX(T));
    if (p == pools_.end()) {
      return false;
    }
    return p->second->Has(entity);
  }

  template <typename T>
  void RemoveComponent(u32 entity) {
    assert(entities_.Has(entity));
    auto p = pools_.find(TYPE_IDX(T));
    assert((p != pools_.end() && p->second->Has(entity)));
    if (p == pools_.end() || !p->second->Has(entity)) {
      return;
    }
    p->second->Remove(entity);
  }

  template <typename T>
  T &GetComponent(u32 entity) {
    assert(pools_.find(TYPE_IDX(T)) != pools_.end());
    auto *pool = static_cast<SparseSet<T> *>(pools_.at(TYPE_IDX(T)).get());

    return pool->Get(entity);
  }

  template <typename... Ts>
  std::vector<u32> Query() {
    u32 min_comps = kMaxId;
    u32 min_idx{0};
    std::vector<ISparseSet *> required_pools;
    std::vector<u32> result{};

    // TODO: Sort pools dynamically when component count changes
    // Find smallest pool to iterate:
    for (auto &t : std::vector<std::type_index>{TYPE_IDX(Ts)...}) {
      auto p = pools_.find(t);
      if (p == pools_.end()) {
        return result;
      }

      required_pools.push_back(p->second.get());
      if (p->second->Count() < min_comps) {
        min_comps = p->second->Count();
        min_idx = required_pools.size() - 1;
      }
    }

    // Iterate smallest pool and add ID to result if it's in all others
    ISparseSet *small_pool = required_pools.at(min_idx);
    for (const auto &id : small_pool->GetAllIds()) {
      bool good = true;
      for (auto *p : required_pools) {
        if (!p->Has(id)) {
          good = false;
          break;
        }
      }
      if (good) {
        result.push_back(id);
      }
    }
    return result;
  }

private:
  static u32 NewId() {
    static u32 s_id = 0;
    return ++s_id;
  }
  template <typename T>
  void AddPool() {
    assert(pools_.find(TYPE_IDX(T)) == pools_.end());
    std::unique_ptr<ISparseSet> s =
        std::unique_ptr<ISparseSet>(new SparseSet<T>);
    pools_[TYPE_IDX(T)] = std::move(s);
  }

  template <typename T>
  SparseSet<T> &GetOrCreatePool() {
    if (pools_.find(TYPE_IDX(T)) == pools_.end()) {
      AddPool<T>();
    }
    SparseSet<T> *s = static_cast<SparseSet<T> *>(pools_.at(TYPE_IDX(T)).get());
    return *s;
  }
};
