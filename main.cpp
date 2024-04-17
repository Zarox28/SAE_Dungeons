#define RAYGUI_IMPLEMENTATION
#define DUNGEON_SIZE 7
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 450

#include "libs/raygui/src/raygui.h"
#include "include/dungeon/dungeon.h"

#include <raylib.h>

int main()
{
  unsigned texture_size = 40;

  Dungeon* d = new Dungeon(DUNGEON_SIZE, DUNGEON_SIZE, 0, 0);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SAE Dungeons - AK | GO | AM");
  SetTargetFPS(60);

  while (! WindowShouldClose())
  {
    if (IsKeyDown(KEY_EQUAL)) texture_size++;
    if (IsKeyDown(KEY_MINUS)) texture_size--;

    
    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    for (unsigned i = 0; i < DUNGEON_SIZE; i++)
    {
      for (unsigned j = 0; j < DUNGEON_SIZE; j++)
      {
        GuiDrawRectangle(
          { .x = (float) texture_size * j + WINDOW_WIDTH / 2
               - (DUNGEON_SIZE * texture_size) / 2,
            .y = (float) texture_size * i + WINDOW_HEIGHT / 2
               - (DUNGEON_SIZE * texture_size) / 2,
            .width  = (float) texture_size,
            .height = (float) texture_size },
          GuiGetStyle(VALUEBOX, BORDER_WIDTH),
          Fade(GetColor(GuiGetStyle(VALUEBOX, BORDER)), guiAlpha),
          WHITE
        );
      }
    }

    EndDrawing();
    
  }
  CloseWindow();
}
