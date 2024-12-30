#include "Application.hpp"
#include "spdlog/spdlog.h"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/event/KeyPressedEvent.hpp"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/platform/LinuxWindow.hpp"

namespace {
std::string kTitle = "Demo";
}

namespace weather {

Application::Application() {
  is_running_ = false;
  s_WindowProps props{kTitle, 1280, 720};
  window_ = std::make_unique<LinuxWindow>(props);

  key_callback_ = [this](const KeyPressedEvent &e) { this->OnKeyPressed(e); };
  resize_callback_ = [this](const ResizeEvent &e) { this->OnResize(e); };
  EventRegister(key_callback_);
  EventRegister(resize_callback_);

  spdlog::info("App started. W: {}, H: {}", window_->GetProps().width,
               window_->GetProps().height);
}

Application::~Application() {
  EventUnregister(key_callback_);
  EventUnregister(resize_callback_);
  spdlog::info("Application shutting down");
}

void Application::Run() {
  spdlog::info("App running");
  is_running_ = true;
  while (is_running_) {
    window_->Update();
  }
}

void Application::OnKeyPressed(const KeyPressedEvent &evt) {
  i32 action = evt.Action();
  i32 key = evt.KeyCode();
  if (action == GLFW_PRESS) {
    spdlog::info("Key {} has been pressed", key);
  } else {
    spdlog::info("Key {} has been released", key);
  }
  if (key == GLFW_KEY_ESCAPE) {
    spdlog::info("App shutting down...");
    window_->Close();
    is_running_ = false;
  }
  if (key == GLFW_KEY_SPACE) {
    spdlog::warn("Removing app resize handler");
    EventUnregister(resize_callback_);
  }
  if (key == GLFW_KEY_O) {
    spdlog::warn("Registering app resize handler");
    EventRegister(resize_callback_);
  }
}

void Application::OnResize(const ResizeEvent &evt) {
  spdlog::info("App recieved ResizeEvent ({}, {}) => ({}, {})", evt.OldSize().w,
               evt.OldSize().h, evt.NewSize().w, evt.NewSize().h);
}
} // namespace weather
