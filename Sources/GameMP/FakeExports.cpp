#include <Engine/Base/NX/FakeExports.h>
class CGame;

/* defs */

extern "C" CGame *GAME_Create(void);

/* exports */

#ifdef FIRST_ENCOUNTER
static CFakeLib exp_Game("Game" DLLPOSTFIX, {
#else
static CFakeLib exp_GameMP("GameMP" DLLPOSTFIX, {
#endif
  { "GAME_Create", (void*)&GAME_Create }, 
});
