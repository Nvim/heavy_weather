#pragma once

#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/ResourceReloadEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/resources/AssetLibrary.hpp"
#include "heavy_weather/resources/Image.hpp"
#include <filesystem>
#include <string>
#include <utility>

namespace weather::graphics {

enum class TextureWrapFlag : u8 {
  REPEAT,
  REPEAT_MIRROR,
  CLAMP_EDGE,
  CLAMP_BORDER
};

enum class TextureFilterFlag : u8 {
  LINEAR,
  NEAREST,
  MIPMAP_NEAREST_NEAREST,
  MIPMAP_NEAREST_LINEAR,
  MIPMAP_LINEAR_LINEAR,
  MIPMAP_LINEAR_NEAREST
};

struct TextureParams {
  TextureWrapFlag Wrap{TextureWrapFlag::REPEAT};
  TextureFilterFlag MinFilter{TextureFilterFlag::MIPMAP_LINEAR_LINEAR};
  TextureFilterFlag MagFilter{TextureFilterFlag::LINEAR};
};

class Texture {
  friend class AssetLibrary<Texture>;

public:
  explicit Texture(SharedPtr<Image> image)
      : img_{std::move(image)}, unit_{counter++} {
    HW_ASSERT(img_ != nullptr);
    HW_ASSERT(!img_->Empty());
    if (img_->Path().has_stem()) {
      SetName(img_->Path().stem().string());
      HW_CORE_DEBUG("Texture created with name {}", name_);
    }
    reload_cb_ = [this](const ResourceReloadEvent<Image> &evt) {
      this->OnResourceReload(evt);
    };
    EventRegister(reload_cb_, this);
  };
  virtual ~Texture() { EventUnregister(reload_cb_); };

  virtual void Bind() const = 0;
  virtual void SetMinFilterFlag(TextureFilterFlag flag) = 0;
  virtual void SetMagFilterFlag(TextureFilterFlag flag) = 0;
  virtual void SetWrapFlag(TextureWrapFlag flag) = 0;
  virtual void SetParams(const TextureParams &params) = 0;
  virtual void Reload() = 0;
  virtual void OnResourceReload(const ResourceReloadEvent<Image> &evt) = 0;
  void SetName(const std::string &name) { name_ = name; }

  i32 Unit() const { return unit_; }
  i32 *UnitPtr() { return &unit_; }
  bool Loaded() const { return loaded_; }
  const std::filesystem::path &Path() const { return img_->Path(); }
  const std::string &Name() const { return name_; }
  std::pair<u32, u32> Size() const { return img_->Size(); };
  u32 Handle() const { return handle_; }

  static std::string ComputeName(const std::filesystem::path &path) {
    return path.stem().string();
  }

private:
  static inline i32 counter{0};
  SharedPtr<Image> img_;
  std::string name_;
  bool loaded_{false};
  u32 handle_{0};
  i32 unit_;
  EventCallback<ResourceReloadEvent<Image>> reload_cb_;

protected:
  u32 *HandlePtr() { return &handle_; }
  Image &GetImage() const { return *img_; }
};

} // namespace weather::graphics
