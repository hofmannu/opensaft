
#include "VectorN.h"
#include <algorithm>
#include <cstdint>
#include <vector>

#pragma once

namespace opensaft {

enum class Dimension { X, Y, Z };

class Volume : std::vector<float> {
public:
  Volume(const Size3& dims) : m_dims(dims) {
    resize(dims[0] * dims[1] * dims[2]);
  }

  /// volumetric access operators (const and non const)
  [[nodiscard]] float& operator()(const std::size_t x, const std::size_t y,
                                  const std::size_t z) {
    return at(x + m_dims[0] * (y + m_dims[1] * z));
  }

  [[nodiscard]] float operator()(const std::size_t x, const std::size_t y,
                                 const std::size_t z) const {
    return at(x + m_dims[0] * (y + m_dims[1] * z));
  }

  /// \return side length along that dimension
  [[nodiscard]] float get_length(const uint8_t iDim) const {
    return m_res[iDim] * m_dims[iDim];
  }

  /// \return the lower edge of the first voxel along that dimension
  [[nodiscard]] float get_minPos(const uint8_t iDim) const {
    return m_center[iDim] - get_length(iDim) * 0.5f;
  }

  /// \return the upper edge of the last voxel along that dimension
  [[nodiscard]] float get_maxPos(const uint8_t iDim) const {
    return m_center[iDim] + get_length(iDim) * 0.5f;
  }

  /// \returns the minimum value held by the data vector
  [[nodiscard]] float get_minVal() const {
    return *std::min_element(begin(), end());
  }

  /// \returns the maximum value held by the data vector
  [[nodiscard]] float get_maxVal() const {
    return *std::max_element(begin(), end());
  }

  /// \returns the resolution of the voxel along a specified dimension
  [[nodiscard]] float get_res(const uint8_t iDim) const { return m_res[iDim]; }

  /// \returns the size of the volume in voxels along a certain dimension
  [[nodiscard]] std::size_t get_dim(const uint8_t iDim) const {
    return m_dims[iDim];
  }

  [[nodiscard]] float get_pos(const std::size_t idx, const uint8_t iDim) const {
    return get_minPos(iDim) + get_res(iDim) * (idx + 0.5f);
  }

  /// \returns reads the volume from a file (shouldbe externalized later)
  void ReadFromFile(const std::string& filePath);

private:
  Float3 m_center{0.0f}; ///!< the center of the 3D volume
  Float3 m_res{1.0f};    ///!< the resolution of the voxels in xyz
  const Size3 m_dims;    ///!< number of voxels along each dimension
};

} // namespace opensaft
