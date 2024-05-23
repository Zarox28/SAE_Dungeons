#include "include/graph/graph.h"

#define RAYGUI_IMPLEMENTATION
#define WINDOW_WIDTH  1300
#define WINDOW_HEIGHT 800

#include "include/dungeon/dungeon.h"
#include "libs/raygui/src/raygui.h"

#include <raylib.h>

int main()
{
  unsigned dungeon_width  = 30;
  unsigned dungeon_height = 30;
  unsigned spinbox_width  = dungeon_width;
  unsigned spinbox_height = dungeon_width;
  unsigned spinbox_x      = 0;
  unsigned spinbox_y      = 0;

  Dungeon* d = new Dungeon(
    dungeon_width, dungeon_height, 0, 0, dungeon_width - 1, dungeon_height - 1
  );
  Graph* g = d->get_graph();

  const unsigned wall_ratio    = 10;
  const unsigned panning_speed = 20;
  unsigned       cell_size     = 15;
  int            offset_x      = 120;
  int            offset_y      = 0;

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SAE Dungeons - AK | GO | AM");
  SetTargetFPS(60);

  while (! WindowShouldClose())
  {
    if (IsKeyDown(KEY_EQUAL)) cell_size++;
    if (IsKeyDown(KEY_MINUS) && cell_size > 1) cell_size--;
    if (IsKeyDown(KEY_LEFT)) offset_x += panning_speed;
    if (IsKeyDown(KEY_RIGHT)) offset_x -= panning_speed;
    if (IsKeyDown(KEY_UP)) offset_y += panning_speed;
    if (IsKeyDown(KEY_DOWN)) offset_y -= panning_speed;
    if (IsKeyDown(KEY_R))
    {
      offset_x  = 120;
      offset_y  = 0;
      cell_size = 15;
    }

    std::vector< unsigned > path = g->DFSFindPath(d->start_cell, d->exit_cell);

    BeginDrawing();
    ClearBackground(BLACK);
    for (unsigned cellY = 0; cellY < dungeon_height; cellY++)
    {
      for (unsigned cellX = 0; cellX < dungeon_width; cellX++)
      {
        const unsigned cellNum   = cellY * dungeon_width + cellX;
        const float    wall_size = (float) cell_size / wall_ratio;
        // TODO: Use textures instead of colored rectangles
        Color wall_color;
        Color cell_color = GRAY;

        if (cellNum == d->start_cell) cell_color = RED;
        if (cellNum == d->exit_cell) cell_color = GREEN;
        if (std::find(path.begin(), path.end(), cellNum) != path.end()) cell_color = RED;

        // Draw cells
        DrawRectangleV(
          { .x = offset_x + (float) cell_size * cellX + WINDOW_WIDTH / 2
               - (dungeon_width * cell_size) / 2,
            .y = offset_y + (float) cell_size * cellY + WINDOW_HEIGHT / 2
               - (dungeon_height * cell_size) / 2 },
          { .x = cell_size - wall_size, .y = cell_size - wall_size },
          cell_color
        );

        // Draw walls, we already calculated the orientation of every wall
        // when generating the base graph, but we redo it again here, the small
        // speed benefit does not outcome the storage space we will need to store it

        if (cellX + 1 < dungeon_width)
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
            default :
              wall_color = BLUE;
              break;  // for debugging, we should never see a blue wall
          }

          DrawRectangleV(
            { .x = offset_x
                 + ((float) cell_size * cellX + WINDOW_WIDTH / 2
                    - (dungeon_width * cell_size) / 2)
                 + cell_size - wall_size,
              .y = offset_y + (float) cell_size * cellY + WINDOW_HEIGHT / 2
                 - (dungeon_height * cell_size) / 2 },
            { .x = wall_size, .y = (float) cell_size - (float) cell_size / wall_ratio },
            wall_color
          );
        }
        if (cellY + 1 < dungeon_width)
        {
          // Get the connection type between the current cell
          // and it's direct bottom one
          unsigned below = cellNum + dungeon_width;

          // Render the wall
          switch (g->get_connection_between(cellNum, below).type)
          {
            case WALLED      : wall_color = BLACK; break;
            case NO_ITEM     : wall_color = GRAY; break;
            case MONSTER     : wall_color = GREEN; break;
            case HEALTH_PACK : wall_color = RED; break;
            case TREASURE    : wall_color = YELLOW; break;
            default :
              wall_color = BLUE;
              break;  // for debugging, we should never see a blue wall
          }

          DrawRectangleV(
            { .x = offset_x + (float) cell_size * cellX + WINDOW_WIDTH / 2
                 - (dungeon_width * cell_size) / 2,
              .y = offset_y
                 + ((float) cell_size * cellY + WINDOW_HEIGHT / 2
                    - (dungeon_height * cell_size) / 2)
                 + cell_size - wall_size },
            { .x = (float) cell_size - (float) cell_size / wall_ratio, .y = wall_size },
            wall_color
          );
        }
      }
    }

    // UI

    DrawRectangleRec({ .x = 0, .y = 0, .width = 250, .height = WINDOW_HEIGHT }, WHITE);

    GuiLabel(Rectangle { .x = 90, .y = 0, .width = 100, .height = 20 }, "Paramètres");
    GuiSpinner(
      Rectangle { .x = 120, .y = 50, .width = 100, .height = 20 },
      "Largeur dongeon   ",
      (int*) &spinbox_width,
      3,
      110,
      false
    );
    GuiSpinner(
      Rectangle { .x = 120, .y = 70, .width = 100, .height = 20 },
      "Hauteur du dongeon",
      (int*) &spinbox_height,
      3,
      110,
      false
    );

    GuiSpinner(
      Rectangle { .x = 30, .y = 20, .width = 90, .height = 20 },
      "X",
      (int*) &spinbox_x,
      0,
      spinbox_width - 1,
      false
    );
    GuiSpinner(
      Rectangle { .x = 135, .y = 20, .width = 90, .height = 20 },
      "Y",
      (int*) &spinbox_y,
      0,
      spinbox_height - 1,
      false
    );

    if (GuiButton(
          Rectangle { .x = 20, .y = 130, .width = 210, .height = 20 },
          "Générer le dongeon"
        ))
    {
      dungeon_width  = spinbox_width;
      dungeon_height = spinbox_height;
      d              = new Dungeon(
        dungeon_width,
        dungeon_height,
        spinbox_x,
        spinbox_y,
        dungeon_width - 1,
        dungeon_height - 1
      );
      g = d->get_graph();
    }

    EndDrawing();
  }
  // TODO: Unload textures

  CloseWindow();
}