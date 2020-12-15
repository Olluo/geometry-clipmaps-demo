/**
 * @file Constants.h
 * @author s5222743
 * @brief This file keeps track of any constants used in Geoclipmapping
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace terraindeformer
{
  const float DEFAULT_HEIGHT = 0.0f;
  const float DEFAULT_COLOUR = 0.9f;
  // K - The level of detail (>2) - 4 for testing, 8 or 16 for final - tested values 4 to 9, any higher and program crashes
  const unsigned char CLIPMAP_K = 9;
  // D - Equivalent of 2^K
  const size_t CLIPMAP_D = 1 << CLIPMAP_K;
  // N - Grid size (must be odd hence -1)
  const size_t CLIPMAP_N = CLIPMAP_D - 1;
  // L - The number of levels of detail - 4 for testing 11 or higher final
  const unsigned char CLIPMAP_L = 4;
  // M -
  const size_t CLIPMAP_M = CLIPMAP_D / 4;
  // H - How much to move the clipmap by to find center point
  const long CLIPMAP_H = -2 * static_cast<int>(CLIPMAP_M) + 1;
} // end namespace terraindeformer
#endif // !CONSTANTS_H_