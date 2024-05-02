#include "include/graph/graph.h"
#define RAYGUI_IMPLEMENTATION
#define DUNGEON_SIZE  40
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#include "include/dungeon/dungeon.h"
#include "libs/raygui/src/raygui.h"

#include <raylib.h>

int main()
{
  Dungeon* d = new Dungeon(DUNGEON_SIZE, DUNGEON_SIZE, 25, 25);
  Graph*   g = d->get_graph();

  const unsigned wall_ratio = 10;
  const unsigned panning_speed = 20;
  unsigned       cell_size  = 40 / (DUNGEON_SIZE / 17);
  int            offset_x   = 0;
  int            offset_y   = 0;

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SAE Dungeons - AK | GO | AM");
  SetTargetFPS(60);

  // Textures
  // TODO: Create and load textures

  while (! WindowShouldClose())
  {
    if (IsKeyDown(KEY_EQUAL)) cell_size++;
    if (IsKeyDown(KEY_MINUS) && cell_size > 1) cell_size--;
    if (IsKeyDown(KEY_LEFT)) offset_x += panning_speed;
    if (IsKeyDown(KEY_RIGHT)) offset_x -= panning_speed;
    if (IsKeyDown(KEY_UP)) offset_y += panning_speed;
    if (IsKeyDown(KEY_DOWN)) offset_y -= panning_speed;

    BeginDrawing();
    ClearBackground(BLACK);
    for (unsigned cellY = 0; cellY < DUNGEON_SIZE; cellY++)
    {
      for (unsigned cellX = 0; cellX < DUNGEON_SIZE; cellX++)
      {
        const unsigned cellNum   = cellY * DUNGEON_SIZE + cellX;
        const float    wall_size = (float) cell_size / wall_ratio;
        // TODO: Use textures instead of colored rectangles
        Color wall_color;
        Color cell_color = GRAY;

        if (cellNum == d->start_cell) cell_color = RED;
        ;

        // Draw cells
        DrawRectangleV(
          { .x = offset_x + (float) cell_size * cellX + WINDOW_WIDTH / 2 - (DUNGEON_SIZE * cell_size) / 2,
            .y = offset_y + (float) cell_size * cellY + WINDOW_HEIGHT / 2 - (DUNGEON_SIZE * cell_size) / 2 },
          { .x = cell_size - wall_size, .y = cell_size - wall_size },
          cell_color
        );

        // Draw walls, we already calculated the orientation of every wall
        // when generated the base graph, but we redo it again here, the small
        // speed benefit does not outcome the storage space we will need to store it

        if (cellX + 1 < DUNGEON_SIZE)
        {
          // Get the the connection type between the current cell
          // and the following one
          unsigned next_on_line = cellNum + 1;

          // Render the wall
          switch (g->get_connection_between(cellNum, next_on_line).type)
          {
            case WALLED      : wall_color = BLACK; break;
            case NO_ITEM     : wall_color = GRAY; break;
            case MONSTER     : wall_color = GREEN; break;
            case HEALTH_PACK : wall_color = RED; break;
            case TREASURE    : wall_color = YELLOW; break;
            default          : wall_color = BLUE; break;  // for debugging, we should never see a blue wall
          }

          DrawRectangleV(
            { .x = offset_x + ((float) cell_size * cellX + WINDOW_WIDTH / 2 - (DUNGEON_SIZE * cell_size) / 2)
                 + cell_size - wall_size,
              .y = offset_y + (float) cell_size * cellY + WINDOW_HEIGHT / 2 - (DUNGEON_SIZE * cell_size) / 2 },
            { .x = wall_size, .y = (float) cell_size - (float) cell_size / wall_ratio },
            wall_color
          );
        }
        if (cellY + 1 < DUNGEON_SIZE)
        {
          // Get the connection type between the current cell
          // and it's direct bottom one
          unsigned below = cellNum + DUNGEON_SIZE;

          // Render the wall
          switch (g->get_connection_between(cellNum, below).type)
          {
            case WALLED      : wall_color = BLACK; break;
            case NO_ITEM     : wall_color = GRAY; break;
            case MONSTER     : wall_color = GREEN; break;
            case HEALTH_PACK : wall_color = RED; break;
            case TREASURE    : wall_color = YELLOW; break;
            default          : wall_color = BLUE; break;  // for debugging, we should never see a blue wall
          }

          DrawRectangleV(
            { .x = offset_x + (float) cell_size * cellX + WINDOW_HEIGHT / 2 - (DUNGEON_SIZE * cell_size) / 2,
              .y = offset_y + ((float) cell_size * cellY + WINDOW_WIDTH / 2 - (DUNGEON_SIZE * cell_size) / 2)
                 + cell_size - wall_size },
            { .x = (float) cell_size - (float) cell_size / wall_ratio, .y = wall_size },
            wall_color
          );
        }
      }
    }
    EndDrawing();
  }
  // TODO: Unload textures

  CloseWindow();
}
