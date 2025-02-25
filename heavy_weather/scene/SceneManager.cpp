#include "SceneManager.hpp"
#include "heavy_weather/event/WidgetCloseEvent.hpp"
#include "heavy_weather/rendering/Gui/GuiComponent.hpp"
#include "heavy_weather/rendering/Gui/IWidget.hpp"
#include <heavy_weather/rendering/Gui/Widgets/MeshWidget.hpp>

namespace weather::graphics {

SceneManager::SceneManager(Renderer &renderer, Gui &gui)
    : gui_{gui}, renderer_{renderer} {}

void SceneManager::AddNode(MeshDescriptor &desc) {
  // construct a mesh using descriptor
  auto mesh = renderer_.CreateMesh(desc);

  // create and register a gui widget for the mesh
  GuiComponentDesc &&color_comp_desc = {mesh->Color(), 0.0f, 0.0f, "color",
                                        nullptr};

  auto transform = mesh->Transform();
  auto cb = [transform]() { transform->dirty = true; };
  GuiComponentDesc transform_comp_desc = {mesh->Transform()->Translation(),
                                          -10.0f, 10.0f, "translation", cb};
  GuiComponentDesc scale_comp_desc = {mesh->Transform()->Scale(), -10.0f, 10.0f,
                                      "scale", cb};
  GuiComponentDesc rotation_comp_desc = {mesh->Transform()->Rotation(), 0.0f,
                                         360.0f, "rotation", cb};

  // register mesh to scene
  auto mesh_id = scene_.AddNode(std::move(mesh));
  auto del_func = [this, mesh_id]() {
    this->scene_.DeleteNode(mesh_id);
    gui_.RemoveWidget(this->nodetogui_[mesh_id]);
    this->nodetogui_.erase(mesh_id);
  };
  GuiComponentDesc delete_comp_desc = {nullptr, 0.0f, 0.0f, "delete", del_func};
  std::unique_ptr<IWidget> p = std::make_unique<MeshWidget>(
      desc.name, std::move(color_comp_desc), std::move(transform_comp_desc),
      std::move(scale_comp_desc), std::move(rotation_comp_desc),
      std::move(delete_comp_desc));
  auto widget_id = gui_.AddWidget(std::move(p));

  // save mapping between mesh & it's gui component
  nodetogui_[mesh_id] = widget_id;
  HW_CORE_DEBUG("Mapped Node #{} to Widget #{}", mesh_id, widget_id);
}

void SceneManager::SubmitAll() {
  auto beg = scene_.GetBegin();
  auto end = scene_.GetEnd();
  for (auto &elem = beg; beg != end; elem++) {
    renderer_.Submit(**elem);
  }
}

} // namespace weather::graphics
