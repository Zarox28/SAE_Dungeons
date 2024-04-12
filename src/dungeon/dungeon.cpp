#pragma once

#include "../../include/dungeon/dungeon.h"

unsigned Dungeon::get_hallway_length() noexcept { return hallway_length; }

void Dungeon::set_hallway_length(unsigned length)
{
  if (length < 3) throw new DungeonInvalidHallwayLengthException(length);
  hallway_length = length;
}

inline unsigned Dungeon::CalculateGridSize() {
  return hallway_length*(graph.get_height() + graph.get_witdh());
}

std::string Dungeon::RenderAsciiFrame() {
  unsigned grid_size = CalculateGridSize();

  // empty frame
  std::string frame (grid_size, ' ');

  return frame;
}

Dungeon::Dungeon(unsigned width, unsigned height, unsigned start_x, unsigned start_y) noexcept(false) : graph(width, height) {
  if (start_x > width || start_y > height) throw new DungeonException("Invalid start position");

  start_node = start_y * width + start_x;
  
  graph.GenerateBaseGraph();
  graph.scramble(start_node);

  graph.DebugGraph();
}


