#pragma once

#include <ftxui/component/component.hpp>

#include "app_context.hpp"

ftxui::Component createFactorizeView(AppContext& app_ctx);
ftxui::Component createFactorizeField(AppContext& app_ctx, const int viewport_height);