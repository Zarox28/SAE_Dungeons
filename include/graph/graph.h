#pragma once

#include "../../include/graph/exceptions.h"

#include <vector>

enum ConnectionItem
{
  DISCONNECTED,
  NO_ITEM,
  MONSTER,
  HEALTH_PACK,
  TREASURE
};

struct Token
{
  ConnectionItem item;
  int            value;
};

class Graph
{
    private:
  unsigned             w;
  unsigned             h;
  std::vector< Token > data;

  static inline bool CanCreateNodeForward(unsigned index_on_line, unsigned grid_width);
  static inline bool CanCreateNodeBottom(
    unsigned line_number, unsigned element_per_line, unsigned grid_height
  );

    public:
  unsigned get_witdh() noexcept;
  unsigned get_height() noexcept;

  void set_width(unsigned width) noexcept(false);
  void set_height(unsigned height) noexcept(false);

  std::vector< Token > get_data() noexcept(false);

  void GenerateBaseGraph() noexcept(false);
  Graph(unsigned width, unsigned height) noexcept(false);
};
