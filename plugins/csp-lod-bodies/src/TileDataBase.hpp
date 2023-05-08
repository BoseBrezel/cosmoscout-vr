////////////////////////////////////////////////////////////////////////////////////////////////////
//                               This file is part of CosmoScout VR                               //
////////////////////////////////////////////////////////////////////////////////////////////////////

// SPDX-FileCopyrightText: German Aerospace Center (DLR) <cosmoscout@dlr.de>
// SPDX-License-Identifier: MIT

#ifndef CSP_LOD_BODIES_TILEBASE_HPP
#define CSP_LOD_BODIES_TILEBASE_HPP

#include "BoundingBox.hpp"
#include "MinMaxPyramid.hpp"
#include "TileDataType.hpp"
#include "TileId.hpp"

#include <memory>
#include <typeinfo>

namespace csp::lodbodies {

/// Abstract base class for data tiles in the HEALPix scheme. A tile stores data samples for a
/// HEALPix patch at a given subdivision level. Actual data is held by classes derived from this
/// one.
class TileDataBase {
 public:
  virtual ~TileDataBase() = default;

  TileDataBase(TileDataBase const& other) = delete;
  TileDataBase(TileDataBase&& other)      = default;

  TileDataBase& operator=(TileDataBase const& other) = delete;
  TileDataBase& operator=(TileDataBase&& other) = default;

  /// Returns the enum value for the data type stored in this tile.
  virtual TileDataType getDataType() const = 0;

  /// Returns read only pointer to data stored in this tile as T*.
  template <typename T>
  T const* getTypedPtr() const;

  /// Returns pointer to data stored in this tile as T*.
  template <typename T>
  T* getTypedPtr();

  /// Returns read only pointer to data stored in this tile.
  virtual void const* getDataPtr() const = 0;

  /// Returns pointer to data stored in this tile.
  virtual void* getDataPtr() = 0;

  /// Returns the resolution given to the tile at construction time.
  uint32_t getResolution() const;

  int  getTexLayer() const;
  void setTexLayer(int layer);

 protected:
  explicit TileDataBase(uint32_t resolution);

 private:
  uint32_t mResolution;
  int      mTexLayer{-1};
};

template <typename T>
T const* TileDataBase::getTypedPtr() const {
  return static_cast<T const*>(getDataPtr());
}

template <typename T>
T* TileDataBase::getTypedPtr() {
  return static_cast<T*>(getDataPtr());
}

} // namespace csp::lodbodies

#endif // CSP_LOD_BODIES_TILEBASE_HPP
