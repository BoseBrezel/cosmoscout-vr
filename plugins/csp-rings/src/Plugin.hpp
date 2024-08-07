////////////////////////////////////////////////////////////////////////////////////////////////////
//                               This file is part of CosmoScout VR                               //
////////////////////////////////////////////////////////////////////////////////////////////////////

// SPDX-FileCopyrightText: German Aerospace Center (DLR) <cosmoscout@dlr.de>
// SPDX-License-Identifier: MIT

#ifndef CSP_RINGS_PLUGIN_HPP
#define CSP_RINGS_PLUGIN_HPP

#include "../../../src/cs-core/PluginBase.hpp"

#include <string>
#include <unordered_map>

namespace csp::rings {

class Ring;

/// This plugin introduces planetary rings. They can be rendered around any object that is
/// located at the center of a spice frame.
/// The configuration of this plugin is done via the provided json config. See README.md for
/// details.
class Plugin : public cs::core::PluginBase {
 public:
  struct Settings {
    /// Settings for a ring around a planet.
    struct Ring {
      /// The path to the texture. The texture should represent a cross section of the ring.
      std::string mTexture;

      /// The distance from the planets center to where the rings start in meter.
      float mInnerRadius{};

      /// The distance from the planets center to where the rings end in meter.
      float mOuterRadius{};
    };

    std::unordered_map<std::string, Ring> mRings;
  };

  void init() override;
  void deInit() override;
  void update() override;

 private:
  void onLoad();
  void onSave();

  Settings                                               mPluginSettings;
  std::unordered_map<std::string, std::shared_ptr<Ring>> mRings;

  int mOnLoadConnection = -1;
  int mOnSaveConnection = -1;
};

} // namespace csp::rings

#endif // CSP_RINGS_PLUGIN_HPP
