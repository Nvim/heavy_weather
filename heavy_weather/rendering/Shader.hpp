#pragma once

#include <utility>

#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/event/EventCallback.hpp"
#include "heavy_weather/event/ResourceReloadEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "heavy_weather/resources/AssetLibrary.hpp"
#include "heavy_weather/resources/ShaderSource.hpp"
namespace weather::graphics {

// Represents a shader file on disk, holds it's shader type,
// Can compile and retrieve errors
class Shader {
  friend class AssetLibrary<ShaderProgram>;

public:
  explicit Shader(ShaderType type, SharedPtr<ShaderSource> src)
      : type_{type}, source_{std::move(src)} {
    HW_ASSERT(source_ != nullptr);
    HW_ASSERT(!source_->Empty());
    reload_cb_ = [this](const ResourceReloadEvent<ShaderSource> &e) {
      this->OnResourceReload(e);
    };
    EventRegister(reload_cb_, this);
  }

  virtual bool Compile() = 0;
  ShaderType Type() const { return type_; }
  ShaderCompileStatus Status() const { return compiled_; }
  const std::filesystem::path &Path() const { return source_->Path(); }
  const std::string &SourceStr() const { return source_->Data(); }
  const ShaderSource &Source() const { return *source_; }

  void ReloadSrc() { source_->Reload(); }
  virtual void
  OnResourceReload(const ResourceReloadEvent<ShaderSource> &evt) = 0;

  //
  virtual ~Shader() { EventUnregister(reload_cb_); };
  Shader(const Shader &) = default;
  Shader(Shader &&) = delete;
  Shader &operator=(const Shader &) = default;
  Shader &operator=(Shader &&) = delete;

protected:
  void SetCompiled(ShaderCompileStatus status) { compiled_ = status; }

private:
  ShaderType type_;
  SharedPtr<ShaderSource> source_;
  ShaderCompileStatus compiled_{NotCompiled}; // Has it been compiled yet
  EventCallback<ResourceReloadEvent<ShaderSource>> reload_cb_;
};

} // namespace weather::graphics
