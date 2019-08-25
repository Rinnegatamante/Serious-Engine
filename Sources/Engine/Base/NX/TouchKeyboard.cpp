#include "Engine/StdH.h"
#include <Engine/Base/NX/TouchKeyboard.h>
#include <switch.h>

CTString CTouchKeyboard::GetInput(const CTString& hint, const CTString& init)
{
  SwkbdConfig kbd;
  char tmp_out[1024] = { 0 };
  Result rc = swkbdCreate(&kbd, 0);
  if (R_SUCCEEDED(rc))
  {
    swkbdConfigMakePresetDefault(&kbd);
    swkbdConfigSetInitialText(&kbd, (const char *)init);
    swkbdConfigSetHeaderText(&kbd, (const char *)hint);
    swkbdConfigSetGuideText(&kbd, (const char *)hint);
    rc = swkbdShow(&kbd, tmp_out, sizeof(tmp_out)-1);
    swkbdClose(&kbd);
    if (R_SUCCEEDED(rc))
      return CTString(tmp_out);
  }
  return init;
}
