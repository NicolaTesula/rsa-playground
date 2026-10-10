#pragma once

#include <ftxui/component/component.hpp>

#include "app_context.hpp"

ftxui::Component createEncryptView(AppContext& app_ctx);
ftxui::Component createEncryptedField(AppContext& app_ctx, const int viewport_height);