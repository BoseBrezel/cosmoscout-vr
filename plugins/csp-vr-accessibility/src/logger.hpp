////////////////////////////////////////////////////////////////////////////////////////////////////
//                               This file is part of CosmoScout VR                               //
////////////////////////////////////////////////////////////////////////////////////////////////////

// SPDX-FileCopyrightText: German Aerospace Center (DLR) <cosmoscout@dlr.de>
// SPDX-License-Identifier: MIT

#ifndef CSP_VR_ACCESSIBILITY_LOGGER_HPP
#define CSP_VR_ACCESSIBILITY_LOGGER_HPP

#include <spdlog/spdlog.h>

namespace csp::vraccessibility {

/// This creates the default singleton logger for "csp-vr-accessibility" when called for the first
/// time and returns it. See cs-utils/logger.hpp for more logging details.
spdlog::logger& logger();

} // namespace csp::vraccessibility

#endif // CSP_VR_ACCESSIBILITY_LOGGER_HPP
