#pragma once

#include "../../include/graph/exceptions.h"
#include <vector>

struct Edge
{
  unsigned vertex_a;
  unsigned vertex_b;
  int      weight_life   = 0;
  int      weight_wealth = 0;
};

inline unsigned CalculateNumberOfEdges(unsigned width, unsigned height);

inline bool CanCreateNodeForward(unsigned index_on_line, unsigned grid_width);

inline bool CanCreateNodeBottom(
  unsigned line_number, unsigned element_per_line, unsigned grid_height
);

extern std::vector< Edge >
GenerateBaseGraph(unsigned width, unsigned height) noexcept(false);
