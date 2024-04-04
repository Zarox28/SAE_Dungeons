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

class Graph
{
    private:
  unsigned            w;
  unsigned            h;
  std::vector< Edge > data;

  static inline unsigned CalculateNumberOfEdges(unsigned width, unsigned height);
  static inline bool CanCreateNodeForward(unsigned index_on_line, unsigned grid_width);
  static inline bool CanCreateNodeBottom(
    unsigned line_number, unsigned element_per_line, unsigned grid_height
  );

    public:
  unsigned get_witdh() noexcept;
  unsigned get_height() noexcept;

  void set_width(unsigned width) noexcept(false);
  void set_height(unsigned height) noexcept(false);

  std::vector< Edge > get_data() noexcept(false);

  void GenerateBaseGraph() noexcept(false);
  Graph(unsigned width, unsigned height) noexcept(false);
};
