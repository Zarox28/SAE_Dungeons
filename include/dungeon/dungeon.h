#pragma once

#include "../../include/graph/graph.h"
#include "./exceptions.h"

#include <string.h>
#include <vector>

class Dungeon
{
    private:
  /// Underlying storage
  Graph graph;

    public:
  /// The entrance to the dungeon, where the path starts
  unsigned start_cell;
  /**
   * Creates a new dungeon
   *
   * @param width Number of cells per line the dungeons has
   * @param height Number of line the dungeons has
   * @param start_x X coordinate of the entrance door to the dungeon
   * @param start_y Y coordinate of the entrance door to the dungeon
   *
   * @throws GraphInvalidSizeException() if the given size of the dungeon is invalid, see Graph constructor for further
   * detail
   * @throws DungeonException() if the given start position is outside of the dungeon
   */
  Dungeon(unsigned width, unsigned height, unsigned start_x, unsigned start_y) noexcept(false);

  /**
   * Renders the dungeon on the terminal
   * This was kindly given by Corentin S and their team
   */
  void   RenderAsciiFrame();
  Graph* get_graph() noexcept;
};
