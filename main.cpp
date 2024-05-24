#include "include/graph/graph.h"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

#define RAYGUI_IMPLEMENTATION
#define WINDOW_WIDTH  1300
#define WINDOW_HEIGHT 800
#define UI_SPACING    20
#define UI_HEIGHT     20

#include "include/dungeon/dungeon.h"
#include "libs/raygui/src/raygui.h"

#include <raylib.h>

int main()
{
  unsigned dungeon_size = 10;
  unsigned spinbox_size = dungeon_size;

  unsigned spinbox_entree_x = 0;
  unsigned spinbox_entree_y = 0;

  unsigned spinbox_exit_x = dungeon_size - 1;
  unsigned spinbox_exit_y = dungeon_size - 1;

  DijkstraConfig config = {};

  Dungeon* d = new Dungeon(dungeon_size, dungeon_size, 0, 0, dungeon_size - 1, dungeon_size - 1);
  Graph*   g = d->get_graph();

  std::vector< unsigned > path_of_exile;
  unsigned                path_index = 0;

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

    // std::vector< unsigned > path = g->DFSFindPath(d->start_cell, d->exit_cell);

    BeginDrawing();
    ClearBackground(BLACK);
    for (unsigned cellY = 0; cellY < dungeon_size; cellY++)
    {
      for (unsigned cellX = 0; cellX < dungeon_size; cellX++)
      {
        const unsigned cellNum   = cellY * dungeon_size + cellX;
        const float    wall_size = (float) cell_size / wall_ratio;

        const float x = offset_x + (float) cell_size * cellX + WINDOW_WIDTH / 2 - (dungeon_size * cell_size) / 2;
        const float y = offset_y + (float) cell_size * cellY + WINDOW_HEIGHT / 2 - (dungeon_size * cell_size) / 2;

        // Simple occlusion;
        // Stop rendering this line of a cell is out of bounds
        if (x > WINDOW_WIDTH) break;
        if (x < 0 || y+cell_size < 0 || y > WINDOW_HEIGHT) continue;  // skip this cell if oob


        // TODO: Use textures instead of colored rectangles
        Color wall_color;
        Color cell_color = GRAY;

        if (cellNum == d->start_cell) cell_color = RED;
        if (cellNum == d->exit_cell) cell_color = GREEN;

        // Draw cells
        DrawRectangleV({ .x = x, .y = y }, { .x = cell_size - wall_size, .y = cell_size - wall_size }, cell_color);

        if (std::find(path_of_exile.begin(), path_of_exile.end(), cellNum) != path_of_exile.end())
          DrawCircle(x + cell_size / 2 - 5 / 2, y + cell_size / 2 - 5 / 2, cell_size / 5, PINK);
        ;

        // if (path_index < path_of_exile.size() && cellNum == path_of_exile[path_index])
        // {
        //   // cell_color = PINK;
        //   DrawCircle(x + cell_size / 2 - 5 / 2, y + cell_size / 2 - 5 / 2, cell_size / 5, PINK);
        //   path_index++;
        //   path_index %= path_of_exile.size();
        // };

        // Draw walls, we already calculated the orientation of every wall
        // when generating the base graph, but we redo it again here, the small
        // speed benefit does not outcome the storage space we will need to store it

        if (cellX + 1 < dungeon_size)
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
            { .x = offset_x + ((float) cell_size * cellX + WINDOW_WIDTH / 2 - (dungeon_size * cell_size) / 2)
                 + cell_size - wall_size,
              .y = offset_y + (float) cell_size * cellY + WINDOW_HEIGHT / 2 - (dungeon_size * cell_size) / 2 },
            { .x = wall_size, .y = (float) cell_size - (float) cell_size / wall_ratio },
            wall_color
          );
        }
        if (cellY + 1 < dungeon_size)
        {
          // Get the connection type between the current cell
          // and it's direct bottom one
          unsigned below = cellNum + dungeon_size;

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
            { .x = offset_x + (float) cell_size * cellX + WINDOW_WIDTH / 2 - (dungeon_size * cell_size) / 2,
              .y = offset_y + ((float) cell_size * cellY + WINDOW_HEIGHT / 2 - (dungeon_size * cell_size) / 2)
                 + cell_size - wall_size },
            { .x = (float) cell_size - (float) cell_size / wall_ratio, .y = wall_size },
            wall_color
          );
        }
      }
    }

    // UI

    DrawRectangleRec({ .x = 0, .y = 0, .width = 250, .height = WINDOW_HEIGHT }, WHITE);
    // DrawRectangleRec({ .x = 0, .y = WINDOW_HEIGHT-50, .width = WINDOW_WIDTH, .height = 50 }, WHITE);

    GuiLabel(Rectangle { .x = 90, .y = 0, .width = 100, .height = 20 }, "Paramètres");
    GuiSpinner(
      Rectangle { .x = 120, .y = UI_SPACING, .width = 100, .height = 20 },
      "Taille dongeon   ",
      (int*) &spinbox_size,
      3,
      110,
      false
    );

    GuiLabel(Rectangle { .x = 10, .y = UI_HEIGHT + UI_SPACING, .width = 100, .height = 20 }, "Position entrée");

    GuiSpinner(
      Rectangle { .x = 30, .y = UI_HEIGHT + UI_SPACING * 2, .width = 90, .height = 20 },
      "X",
      (int*) &spinbox_entree_x,
      0,
      spinbox_size - 1,
      false
    );
    GuiSpinner(
      Rectangle { .x = 135, .y = UI_HEIGHT + UI_SPACING * 2, .width = 90, .height = 20 },
      "Y",
      (int*) &spinbox_entree_y,
      0,
      spinbox_size - 1,
      false
    );

    GuiLabel(Rectangle { .x = 10, .y = UI_HEIGHT + UI_SPACING * 3, .width = 100, .height = 20 }, "Position sortie");

    GuiSpinner(
      Rectangle { .x = 30, .y = UI_HEIGHT + UI_SPACING * 4, .width = 90, .height = 20 },
      "X",
      (int*) &spinbox_exit_x,
      0,
      spinbox_size - 1,
      false
    );
    GuiSpinner(
      Rectangle { .x = 135, .y = UI_HEIGHT + UI_SPACING * 4, .width = 90, .height = 20 },
      "Y",
      (int*) &spinbox_exit_y,
      0,
      spinbox_size - 1,
      false
    );

    if (GuiButton(
          Rectangle { .x = 20, .y = UI_HEIGHT + UI_SPACING * 5.5, .width = 210, .height = 20 }, "Générer le dongeon"
        ))
    {
      dungeon_size = spinbox_size;
      path_index   = 0;
      path_of_exile.clear();
      // dungeon_height = spinbox_size;
      d = new Dungeon(dungeon_size, dungeon_size, spinbox_entree_x, spinbox_entree_y, spinbox_exit_x, spinbox_exit_y);
      g = d->get_graph();
    }

    GuiSlider(
      Rectangle { .x = 115, .y = UI_HEIGHT + UI_SPACING * 7, .width = 90, .height = 20 },
      "Priorité Vie: 0",
      "5",
      &config.weight_health,
      0,
      5
    );
    GuiSlider(
      Rectangle { .x = 115, .y = UI_HEIGHT + UI_SPACING * 8.5, .width = 90, .height = 20 },
      "Priorité Trésore: 0",
      "5",
      &config.weight_money,
      0,
      5
    );

    if (GuiButton(
          Rectangle { .x = 20, .y = UI_HEIGHT + UI_SPACING * 10, .width = 210, .height = 20 }, "Trouver chemin"
        ))
    {
      path_index = 0;
      path_of_exile.clear();

      const std::vector< unsigned > previouses = g->dijkstra(0, g, config);
      for (unsigned i = 0; i < previouses.size(); i++)
        std::cout << "Node: " << i << " Previous: " << previouses[i] << std::endl;
      unsigned current_node = d->exit_cell;
      path_of_exile.clear();

      while (current_node != d->start_cell)
      {
        path_of_exile.push_back(current_node);
        // printf("%d->", current_node);
        current_node = previouses[current_node];
      }
    }
    EndDrawing();
  }
  CloseWindow();
}
