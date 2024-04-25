#define RAYGUI_IMPLEMENTATION
#define DUNGEON_SIZE  3
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#include "include/dungeon/dungeon.h"
#include "libs/raygui/src/raygui.h"

#include <raylib.h>


int main()
{
  Dungeon* d = new Dungeon(DUNGEON_SIZE, DUNGEON_SIZE, 0, 0);
  d->RenderAsciiFrame();
}
