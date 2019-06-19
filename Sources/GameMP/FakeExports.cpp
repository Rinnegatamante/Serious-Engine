#include <Engine/Base/NX/FakeExports.h>
class CGame;

/* defs */

extern "C" CGame *GAME_Create(void);

/* exports */

static CFakeLib exp_GameMP("GameMP", {
  { "GAME_Create", (void*)&GAME_Create }, 
});
