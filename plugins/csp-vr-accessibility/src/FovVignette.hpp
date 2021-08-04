////////////////////////////////////////////////////////////////////////////////////////////////////
//                               This file is part of CosmoScout VR                               //
//      and may be used under the terms of the MIT license. See the LICENSE file for details.     //
//                        Copyright: (c) 2019 German Aerospace Center (DLR)                       //
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CSP_VR_ACCESSIBILITY_FOVVIGNETTE_HPP
#define CSP_VR_ACCESSIBILITY_FOVVIGNETTE_HPP

#include "Plugin.hpp"

#include "../../../src/cs-scene/CelestialObject.hpp"
#include "../../../src/cs-utils/AnimatedValue.hpp"

#include <VistaKernel/GraphicsManager/VistaOpenGLDraw.h>
#include <VistaKernel/GraphicsManager/VistaOpenGLNode.h>
#include <VistaKernel/GraphicsManager/VistaTransformNode.h>
#include <VistaOGLExt/VistaBufferObject.h>
#include <VistaOGLExt/VistaGLSLShader.h>
#include <VistaOGLExt/VistaTexture.h>
#include <VistaOGLExt/VistaVertexArrayObject.h>
#include <VistaKernel/DisplayManager/VistaViewport.h>

namespace cs::core {
class Settings;
class SolarSystem;
} // namespace cs::core

namespace csp::vraccessibility {
/// The FoV Vignette. It draws a vignette when the observer is moving.
class FovVignette : public IVistaOpenGLDraw{
 public:
  FovVignette(std::shared_ptr<cs::core::SolarSystem> solarSystem);

  FovVignette(FovVignette const& other) = delete;
  FovVignette(FovVignette&& other) = default;

  FovVignette& operator=(FovVignette const& other) = delete;
  FovVignette& operator=(FovVignette&& other) = delete;

  ~FovVignette() override;

  /// Configures the internal renderer according to the given values.
  void configure(std::shared_ptr<Plugin::Settings> settings);

  /// Updates the variables for the Shaders
  void updateDynamicRadiusVignette();
  void updateFadeAnimatedVignette();

  bool Do() override;
  bool GetBoundingBox(VistaBoundingBox& bb) override;

 private:
  std::shared_ptr<cs::core::Settings>     mSettings;
  std::shared_ptr<cs::core::SolarSystem>  mSolarSystem;

  std::unique_ptr<VistaOpenGLNode>        mGLNode;

  cs::utils::AnimatedValue<float>                     mFadeAnimation;
  double                                              mLastChange;
  int                                                 mAnimationTracker;
  bool                                                mIsStill;

  float                                               mCurrentInnerRadius;
  float                                               mCurrentOuterRadius;
  float                                               mLastInnerRadius = 1.4142F;
  float                                               mLastOuterRadius = 1.4142F;
  std::chrono::time_point<std::chrono::high_resolution_clock>  mLastTime;
  float                                               mNormalizedVelocity;

  float getNewRadius(float innerOuterRadius, float normVelocity, float lastRadius, double dT);

  std::shared_ptr<Plugin::Settings>       mVignetteSettings;
  VistaGLSLShader                         mShaderFade;
  VistaGLSLShader                         mShaderDynRad;
  VistaGLSLShader                         mShaderFadeVertOnly;
  VistaGLSLShader                         mShaderDynRadVertOnly;
  VistaVertexArrayObject                  mVAO;
  VistaBufferObject                       mVBO;

  struct GBufferData {
    std::unique_ptr<VistaTexture> mDepthBuffer;
    std::unique_ptr<VistaTexture> mColorBuffer;
  };

  std::unordered_map<VistaViewport*, GBufferData> mGBufferData;

  static const char* VERT_SHADER;
  static const char* FRAG_SHADER_FADE;
  static const char* FRAG_SHADER_DYNRAD;
  static const char* FRAG_SHADER_FADE_VERTONLY;
  static const char* FRAG_SHADER_DYNRAD_VERTONLY;
}; // class FloorGrid
} // namespace csp::vraccessibility

#endif // CSP_VR_ACCESSIBILITY_FOVVIGNETTE_HPP