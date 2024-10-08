// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_DISPLAY_LIST_DL_COLOR_H_
#define FLUTTER_DISPLAY_LIST_DL_COLOR_H_

#include "flutter/display_list/geometry/dl_geometry_types.h"

namespace flutter {

struct DlColor {
 public:
  constexpr DlColor() : argb_(0xFF000000) {}
  constexpr explicit DlColor(uint32_t argb) : argb_(argb) {}

  /// @brief Construct a 32 bit color from floating point R, G, B, and A color
  /// channels.
  static constexpr DlColor RGBA(DlScalar r,
                                DlScalar g,
                                DlScalar b,
                                DlScalar a) {
    return ARGB(a, r, g, b);
  }

  /// @brief Construct a 32 bit color from floating point A, R, G, and B color
  /// channels.
  static constexpr DlColor ARGB(DlScalar a,
                                DlScalar r,
                                DlScalar g,
                                DlScalar b) {
    return DlColor(toC(a) << 24 |  //
                   toC(r) << 16 |  //
                   toC(g) << 8 |   //
                   toC(b));
  }

  static constexpr uint8_t toAlpha(DlScalar opacity) { return toC(opacity); }
  static constexpr DlScalar toOpacity(uint8_t alpha) { return toF(alpha); }

  // clang-format off
  static constexpr DlColor kTransparent()        {return DlColor(0x00000000);};
  static constexpr DlColor kBlack()              {return DlColor(0xFF000000);};
  static constexpr DlColor kWhite()              {return DlColor(0xFFFFFFFF);};
  static constexpr DlColor kRed()                {return DlColor(0xFFFF0000);};
  static constexpr DlColor kGreen()              {return DlColor(0xFF00FF00);};
  static constexpr DlColor kBlue()               {return DlColor(0xFF0000FF);};
  static constexpr DlColor kCyan()               {return DlColor(0xFF00FFFF);};
  static constexpr DlColor kMagenta()            {return DlColor(0xFFFF00FF);};
  static constexpr DlColor kYellow()             {return DlColor(0xFFFFFF00);};
  static constexpr DlColor kDarkGrey()           {return DlColor(0xFF3F3F3F);};
  static constexpr DlColor kMidGrey()            {return DlColor(0xFF808080);};
  static constexpr DlColor kLightGrey()          {return DlColor(0xFFC0C0C0);};
  static constexpr DlColor kAliceBlue()          {return DlColor(0xFFF0F8FF);};
  static constexpr DlColor kFuchsia()            {return DlColor(0xFFFF00FF);};
  static constexpr DlColor kMaroon()             {return DlColor(0xFF800000);};
  static constexpr DlColor kSkyBlue()            {return DlColor(0xFF87CEEB);};
  static constexpr DlColor kCornflowerBlue()     {return DlColor(0xFF6495ED);};
  static constexpr DlColor kCrimson()            {return DlColor(0xFFFF5733);};
  static constexpr DlColor kAqua()               {return DlColor(0xFF00FFFF);};
  static constexpr DlColor kOrange()             {return DlColor(0xFFFFA500);};
  static constexpr DlColor kPurple()             {return DlColor(0xFF800080);};
  static constexpr DlColor kLimeGreen()          {return DlColor(0xFF32CD32);};
  static constexpr DlColor kGreenYellow()        {return DlColor(0xFFADFF2F);};
  static constexpr DlColor kDarkMagenta()        {return DlColor(0xFF8B008B);};
  static constexpr DlColor kOrangeRed()          {return DlColor(0xFFFF4500);};
  // clang-format on

  constexpr bool isOpaque() const { return getAlpha() == 0xFF; }
  constexpr bool isTransparent() const { return getAlpha() == 0; }

  constexpr int getAlpha() const { return argb_ >> 24; }
  constexpr int getRed() const { return (argb_ >> 16) & 0xFF; }
  constexpr int getGreen() const { return (argb_ >> 8) & 0xFF; }
  constexpr int getBlue() const { return argb_ & 0xFF; }

  constexpr DlScalar getAlphaF() const { return toF(getAlpha()); }
  constexpr DlScalar getRedF() const { return toF(getRed()); }
  constexpr DlScalar getGreenF() const { return toF(getGreen()); }
  constexpr DlScalar getBlueF() const { return toF(getBlue()); }

  constexpr uint32_t premultipliedArgb() const {
    if (isOpaque()) {
      return argb_;
    }
    DlScalar f = getAlphaF();
    return (argb_ & 0xFF000000) |       //
           toC(getRedF() * f) << 16 |   //
           toC(getGreenF() * f) << 8 |  //
           toC(getBlueF() * f);
  }

  constexpr DlColor withAlpha(uint8_t alpha) const {  //
    return DlColor((argb_ & 0x00FFFFFF) | (alpha << 24));
  }
  constexpr DlColor withRed(uint8_t red) const {  //
    return DlColor((argb_ & 0xFF00FFFF) | (red << 16));
  }
  constexpr DlColor withGreen(uint8_t green) const {  //
    return DlColor((argb_ & 0xFFFF00FF) | (green << 8));
  }
  constexpr DlColor withBlue(uint8_t blue) const {  //
    return DlColor((argb_ & 0xFFFFFF00) | (blue << 0));
  }

  constexpr DlColor modulateOpacity(DlScalar opacity) const {
    return opacity <= 0   ? withAlpha(0)
           : opacity >= 1 ? *this
                          : withAlpha(round(getAlpha() * opacity));
  }

  constexpr uint32_t argb() const { return argb_; }

  bool operator==(DlColor const& other) const { return argb_ == other.argb_; }
  bool operator!=(DlColor const& other) const { return argb_ != other.argb_; }
  bool operator==(uint32_t const& other) const { return argb_ == other; }
  bool operator!=(uint32_t const& other) const { return argb_ != other; }

 private:
  uint32_t argb_;

  static constexpr DlScalar toF(uint8_t comp) { return comp * (1.0f / 255); }
  static constexpr uint8_t toC(DlScalar fComp) { return round(fComp * 255); }
};

}  // namespace flutter

#endif  // FLUTTER_DISPLAY_LIST_DL_COLOR_H_
