#pragma once

#include "../../include/dungeon/dungeon.h"
#include <iostream>

Graph* Dungeon::get_graph() noexcept {
    return &graph;
}

Dungeon::Dungeon(unsigned width, unsigned height, unsigned start_x, unsigned start_y) noexcept(false) :
  graph(width, height)
{
  if (start_x > width - 1 || start_y > height - 1) throw new DungeonException("Invalid start position");

  start_cell = start_y * width + start_x;

  graph.GenerateBaseGraph();
  exit_cell = graph.Scramble(start_cell);
  // graph.PrintAjacencyMatrix();
}
