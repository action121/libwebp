// Copyright 2012 Google Inc. All Rights Reserved.
//
// This code is licensed under the same terms as WebM:
//  Software License Agreement:  http://www.webmproject.org/license/software/
//  Additional IP Rights Grant:  http://www.webmproject.org/license/additional/
// -----------------------------------------------------------------------------
//
// Image transforms and color space conversion methods for lossless decoder.
//
// Author: Vikas Arora (vikaas.arora@gmail.com)
//         jyrki@google.com (Jyrki Alakuijala)

#ifndef WEBP_DSP_LOSSLESS_H_
#define WEBP_DSP_LOSSLESS_H_

#include "../webp/types.h"
#include "../webp/decode.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

//------------------------------------------------------------------------------
// Image transforms.

struct VP8LTransform;  // Defined in dec/vp8li.h.

// Performs inverse transform of data given transform information, start and end
// rows. Transform will be applied to rows [row_start, row_end[.
// The data_in & data_out are source and destination data pointers respectively
// corresponding to the intermediate row (row_start).
void VP8LInverseTransform(const struct VP8LTransform* const transform,
                          int row_start, int row_end,
                          uint32_t* const data_in, uint32_t* const data_out);

#ifdef USE_LOSSLESS_ENCODER
// Subtracts green from blue and red channels.
void VP8LSubtractGreenFromBlueAndRed(uint32_t* argb_data, int num_pixs);

void VP8LResidualImage(int width, int height, int bits,
                       uint32_t* const argb, uint32_t* const argb_scratch,
                       uint32_t* const image);

void VP8LColorSpaceTransform(int width, int height, int bits, int step,
                             uint32_t* const argb, uint32_t* image);
#endif

//------------------------------------------------------------------------------
// Color space conversion.

// Converts from BGRA to other color spaces.
void VP8LConvertFromBGRA(const uint32_t* const in_data, int num_pixels,
                         WEBP_CSP_MODE out_colorspace,
                         uint8_t* const rgba);

//------------------------------------------------------------------------------
// Misc methods.

// Computes sampled size of 'size' when sampling using 'sampling bits'.
static WEBP_INLINE uint32_t VP8LSubSampleSize(uint32_t size,
                                              uint32_t sampling_bits) {
  return (size + (1 << sampling_bits) - 1) >> sampling_bits;
}

#ifdef USE_LOSSLESS_ENCODER
// Faster logarithm for small integers, with the property of log(0) == 0.
float VP8LFastLog(int v);

// In-place difference of each component with mod 256.
static WEBP_INLINE uint32_t VP8LSubPixels(uint32_t a, uint32_t b) {
  const uint32_t alpha_and_green =
      0x00ff00ffu + (a & 0xff00ff00u) - (b & 0xff00ff00u);
  const uint32_t red_and_blue =
      0xff00ff00u + (a & 0x00ff00ffu) - (b & 0x00ff00ffu);
  return (alpha_and_green & 0xff00ff00u) | (red_and_blue & 0x00ff00ffu);
}
#endif

//------------------------------------------------------------------------------

#if defined(__cplusplus) || defined(c_plusplus)
}    // extern "C"
#endif

#endif  // WEBP_DSP_LOSSLESS_H_