#pragma once

#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/resources/Loader.hpp"
#include <algorithm>
#include <unordered_map>

namespace weather {

template <typename T> class AssetLibrary {
public:
  // Add if not present, else do nothing
  void Add(SharedPtr<T> data) {
    if (!Has(data.get()->Name())) {
      HW_CORE_DEBUG("AssetLibrary: Adding asset {}.", data.get()->Name());
      // assets_.at(data.get()->Name()) = data;
      assets_[data.get()->Name()] = data;
    }
  }

  // Load resource from disk, add it to library and return ptr to it
  SharedPtr<T> Load(const std::string &path) {
    if (!Has(path)) {
      HW_CORE_DEBUG("AssetLibrary: Loading resource {} using Loader", path);
      assets_[path] = Loader<T>::Load(path);
    }
    return assets_[path];
  }

  // Reload if present, else add
  void Reload(SharedPtr<T> data) { assets_.at(data.get()->Name()) = data; }

  bool Has(const std::string &name) {
    return (assets_.find(name) != assets_.end());
  }

  bool HasPath(const std::filesystem::path &path) {
    auto r =
        std::find_if(assets_.begin(), assets_.end(), [&path](const auto &elem) {
          return elem.second->Path() == path;
        });
    return r != assets_.end();
  }

  // nullptr if absent
  SharedPtr<T> Get(const std::string &name) {
    auto at = assets_.find(name);
    if (at != assets_.end()) {
      return at->second;
    }
    std::string errmsg = fmt::format("Couldn't find asset {}", name);
    HW_ASSERT_MSG(false, errmsg.c_str());
    return nullptr;
  }

  SharedPtr<T> GetPath(const std::filesystem::path &path) {
    auto r =
        std::find_if(assets_.begin(), assets_.end(), [&path](const auto &elem) {
          HW_CORE_WARN("Comparing paths {} and {}", path.c_str(),
                       elem.second->Path().c_str());
          return (elem.second->Path() == path);
        });
    if (r != assets_.end()) {
      return r->second;
    }
    return nullptr;
  }

  const std::unordered_map<std::string, SharedPtr<T>> &GetAll() const {
    return assets_;
  }

  void OnGuiRender();

private:
  std::unordered_map<std::string, SharedPtr<T>> assets_;
};

} // namespace weather
