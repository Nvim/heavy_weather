#include "Application.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Input/InputSystem.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/platform/Platform.hpp"
#include "heavy_weather/rendering/Gui/Gui.hpp"
#include <heavy_weather/rendering/Backend/GL/GLAPI.hpp>
#include <heavy_weather/rendering/Shader.hpp>
#include <heavy_weather/rendering/ShaderProgram.hpp>
#include <heavy_weather/rendering/Types.hpp>

namespace weather {

Application::Application(WindowProps &window_props, f64 fps) {
  HW_ASSERT_MSG(!s_instance, "App already exists");
  s_instance = this;
  is_running_ = false;
  fps_ = fps;
  window_ = PlatformInitWindow(window_props);

  if (!InputSystem::Init(window_->GetNative())) {
    HW_CORE_CRITICAL("Failed to init input");
  }

#ifdef HW_ENABLE_GUI
  graphics::Gui::Init({graphics::Backend::OpenGL, window_->GetNative()});
#endif
  resize_callback_ = [this](const ResizeEvent &e) { this->OnResize(e); };
  close_callback_ = [this](const WindowCloseEvent &e) { this->OnClose(e); };
  EventRegister(resize_callback_, this);
  EventRegister(close_callback_, this);

  HW_CORE_INFO("App started. W: {}, H: {}", window_->GetProps().width,
               window_->GetProps().height);
}

Application::~Application() {
  EventUnregister(resize_callback_);
  EventUnregister(close_callback_);
  EventSystem::Shutdown();
  HW_CORE_INFO("Application shutting down");
}

const Window &Application::GetWindow() const { return *window_; }

void Application::Run() {
  HW_CORE_INFO("App running");
  is_running_ = true;
  f64 start{}, end{}, remaining{}, delta{};

  while (is_running_) {
    start = PlatformGetTime();

    window_->Update();
    this->OnRender(delta);
#ifdef HW_ENABLE_GUI
    // TODO cool version string system
    const char *engine_str = "Heavy Weather Engine - v0.0.0";
    graphics::AppInfo info = {
        this->GetProgramName(),
        engine_str,
        delta,
        delta + remaining,
    };

    graphics::Gui::BeginFrame();
    graphics::Gui::RenderAppWindow(info, (void *)&this->GetWindow());

    this->OnGuiRender(delta);
    graphics::Gui::EndFrame();
#endif // HW_ENABLE_GUI
    end = PlatformGetTime();
    delta = end - start;
    remaining = fps_ - delta;
    if (delta > 0.0f && remaining > 0.0f) {
      // HW_CORE_INFO("Sleeping for {}", remaining);
      PlatformSleep(remaining - 1); // NOLINT
    }
  }
#ifdef HW_ENABLE_GUI
  graphics::Gui::ShutDown();
#endif
}

void Application::OnResize(const ResizeEvent &evt) // NOLINT
{
  (void)evt;
  // HW_CORE_INFO("App recieved ResizeEvent ({}, {}) => ({}, {})",
  // evt.OldSize().w,
  //              evt.OldSize().h, evt.NewSize().w, evt.NewSize().h);
}

void Application::OnClose(const WindowCloseEvent &evt) {
  (void)evt;
  HW_CORE_INFO("App recieved WindowClosedEvent");
  window_->Close();
  is_running_ = false;
}
} // namespace weather
