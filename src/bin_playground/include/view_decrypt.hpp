#pragma once

#include <ftxui/component/component.hpp>

#include "app_context.hpp"

ftxui::Component createDecryptView(AppContext& app_ctx);
ftxui::Component createDecryptedField(AppContext& app_ctx, const int viewport_height);
ftxui::Component createDecryptButton(AppContext& app_ctx);