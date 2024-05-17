#pragma once

#include "../../include/dungeon/dungeon.h"


Graph* Dungeon::get_graph() noexcept {
    return &graph;
}

Dungeon::Dungeon(unsigned width, unsigned height, unsigned start_x, unsigned start_y, unsigned end_x, unsigned end_y) noexcept(false) :
  graph(width, height)
{
  if (start_x > width - 1 || start_y > height - 1) throw new DungeonException("Invalid start position");
  if (end_x > width - 1 || end_y > height - 1) throw new DungeonException("Invalid end position");


  start_cell = start_y * width + start_x;
  exit_cell = end_y * width + end_x;

  graph.GenerateBaseGraph();
  graph.Scramble(start_cell, exit_cell);
  graph.RemoveRandomWalls(10);
  // graph.PrintAjacencyMatrix();
}
