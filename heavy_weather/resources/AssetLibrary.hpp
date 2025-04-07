#pragma once

/* *
 * Dumb wrapper around a map of {name -> pointer}. This centralises ownership
 * of assets of a similar type in the same class, allowing for future memory
 * management systems.
 * This class is meant to be driven by the `AssetManager`, which adds
 * functionality by having access to a renderer for GPU resource creation.
 *
 * TODO: Look into better memory usage.
 * TODO: Look into returning weak pointers instead of shared, to emphasize
 * ownership.
 * */

#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/resources/Loader.hpp"
#include <algorithm>
#include <unordered_map>

namespace weather {

template <typename T> class AssetLibrary {
public:
  /**
   * @brief Add an asset to the library via pointer.
   *        Does not replace existing assets.
   * @param data: the asset to add.
   */
  void Add(SharedPtr<T> data) {
    if (!Has(data.get()->Name())) {
      HW_CORE_DEBUG("AssetLibrary: Adding asset {}.", data.get()->Name());
      // assets_.at(data.get()->Name()) = data;
      assets_[data.get()->Name()] = data;
    }
  }

  /**
   * @brief Loads resource from disk, adds it to library and return ptr to it.
   *        Does not replace existing assets.
   *        Only loadable types are Image and ShaderSource. Other types
   *        require processing handled by AssetManager.
   * @param path: Path of the resource to load
   * @return: Pointer to the newly added resource
   */
  SharedPtr<T> Load(const std::string &path) {
    if (!Has(path)) {
      HW_CORE_DEBUG("AssetLibrary: Loading resource {} using Loader", path);
      assets_[path] = Loader<T>::Load(path);
    }
    return assets_[path];
  }

  /**
   * @brief Checks if an element is present in the map.
   *        NOTE: This performs a search which can be inefficient if redone to
   *        get the element. In case this is needed, prefer `Get` and check for
   *        nullptr.
   * @param name: Name of the element to look for
   * @return: true if found, false otherwise.
   */
  bool Has(const std::string &name) {
    return (assets_.find(name) != assets_.end());
  }

  /**
   * @brief Similar to Has, but uses a `Path` as parameter, assuming T has a
   *        `Path()` method.
   *        NOTE: This performs a search which can be inefficient if redone to
   *        get the element. In case this is needed, prefer `GetPath` and check
   *        for nullptr.
   * @param path: Path to search for among assets.
   * @return: true if path is found, false otherwise.
   */
  bool HasPath(const std::filesystem::path &path) {
    auto r =
        std::find_if(assets_.begin(), assets_.end(), [&path](const auto &elem) {
          return elem.second->Path() == path;
        });
    return r != assets_.end();
  }

  /**
   * @brief Get a pointer to the asset.
   *
   * @param name: Name of the element to look for
   * @return: Pointer to the element, or nullptr if not found.
   */
  SharedPtr<T> Get(const std::string &name) {
    auto at = assets_.find(name);
    if (at != assets_.end()) {
      return at->second;
    }
    HW_CORE_WARN("Couldn't find asset {}", name);
    return nullptr;
  }

  /**
   * @brief Similar to Get, but uses a `Path` as parameter, assuming T has a
   *        `Path()` method.
   * @param name: Name of the element to look for
   * @return: Pointer to the element, or nullptr if not found.
   */
  SharedPtr<T> GetPath(const std::filesystem::path &path) {
    auto r =
        std::find_if(assets_.begin(), assets_.end(), [&path](const auto &elem) {
          return (elem.second->Path() == path);
        });
    if (r != assets_.end()) {
      return r->second;
    }
    HW_CORE_WARN("Couldn't find asset with path {}", path.c_str());
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
