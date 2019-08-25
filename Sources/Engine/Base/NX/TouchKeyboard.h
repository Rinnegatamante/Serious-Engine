#ifndef SE_INCL_TOUCHKEYBOARD_H
#define SE_INCL_TOUCHKEYBOARD_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/CTString.h>

class CTouchKeyboard {
public:
  static CTString GetInput(const CTString& hint, const CTString& init);
};

#endif // SE_INCL_TOUCHKEYBOARD_H
