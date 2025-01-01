#include "Application.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/platform/LinuxWindow.hpp"

namespace {
std::string kTitle = "Demo";
}

namespace weather {

Application::Application() {
  HW_ASSERT_MSG(!s_instance, "App already exists");
  s_instance = this;
  is_running_ = false;
  s_WindowProps props{kTitle, 1280, 720};
  window_ = std::make_unique<LinuxWindow>(props);

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
  while (is_running_) {
    window_->Update();
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
