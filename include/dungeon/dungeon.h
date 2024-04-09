#pragma once

#include "../../src/graph/graph.cpp"
#include "./exceptions.h"

#include <string.h>
#include <vector>

class Dungeon
{
    private:
  unsigned        hallway_length = 3;
  Graph           graph;
  inline unsigned CalculateGridSize();

    public:
  Dungeon(unsigned width, unsigned height) noexcept(false);
  void        set_hallway_length(unsigned length) noexcept(false);
  unsigned    get_hallway_length() noexcept;
  std::string RenderAsciiFrame();
};