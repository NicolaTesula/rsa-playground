#pragma once

#include <functional>
#include <ftxui/component/component.hpp>

#include "app_context.hpp"

ftxui::Component createMainMenuView(AppContext& app_context, std::function<void(int)> on_select);