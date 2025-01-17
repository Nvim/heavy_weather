#include "Application.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Input/InputSystem.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/platform/Platform.hpp"

namespace {
std::string kTitle = "Sandbox";
constexpr f64 kFrametime = 1.0f / 60;
} // namespace

namespace weather {

Application::Application() {
  HW_ASSERT_MSG(!s_instance, "App already exists");
  s_instance = this;
  is_running_ = false;
  s_WindowProps props{kTitle, 1280, 720};
  window_ = platform_init_window(props);

  if (!InputSystem::Init(window_->GetNative())) {
    HW_CORE_CRITICAL("Failed to init input");
  }
  resize_callback_ = [this](const ResizeEvent &e) { this->OnResize(e); };
  close_callback_ = [this](const WindowCloseEvent &e) { this->OnClose(e); };
  EventRegister(resize_callback_);
  EventRegister(close_callback_);

  HW_CORE_INFO("App started. W: {}, H: {}", window_->GetProps().width,
               window_->GetProps().height);
}

Application::~Application() {
  EventUnregister(resize_callback_);
  EventUnregister(close_callback_);
  HW_CORE_INFO("Application shutting down");
}

const Window &Application::GetWindow() const { return *window_.get(); }

void Application::Run() {
  HW_CORE_INFO("App running");
  is_running_ = true;
  f64 start, end, remaining, delta;
  while (is_running_) {
    start = platform_get_time();
    window_->Update();
    end = platform_get_time();
    delta = end - start;
    remaining = kFrametime - delta;
    if (delta > 0.0f && remaining > 0.0f) {
      platform_sleep(remaining - 1);
    }
  }
}

void Application::OnResize(const ResizeEvent &evt) {
  HW_CORE_INFO("App recieved ResizeEvent ({}, {}) => ({}, {})", evt.OldSize().w,
               evt.OldSize().h, evt.NewSize().w, evt.NewSize().h);
}

void Application::OnClose(const WindowCloseEvent &evt) {
  (void)evt;
  HW_CORE_INFO("App recieved WindowClosedEvent");
  window_->Close();
  is_running_ = false;
}
} // namespace weather
