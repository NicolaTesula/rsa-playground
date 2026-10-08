#pragma once

#include <functional>
#include <ftxui/component/component.hpp>

#include "app_context.hpp"

ftxui::Component createPrimeGenView(AppContext& context, std::function<void()> on_back);