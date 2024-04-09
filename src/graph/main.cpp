#pragma once
#include <iostream>

#include "../../include/graph/graph.h"

inline unsigned CalculateNumberOfEdges(unsigned width, unsigned height)
{
  unsigned edges_per_line   = width - 1;
  unsigned edges_per_column = width;
  unsigned total_edges      = edges_per_line * height + edges_per_column * (height - 1);

  return total_edges;
}

inline bool CanCreateNodeForward(unsigned index_on_line, unsigned grid_width)
{
  return index_on_line + 1 < grid_width;
}

inline bool CanCreateNodeBottom(unsigned line_number, unsigned element_per_line, unsigned grid_height)
{
  return (line_number + 1) * element_per_line < grid_height;
}

extern std::vector<Edge>
GenerateBaseGraph(unsigned width, unsigned height) noexcept(false)
{
  if (width == 0 || height == 0) throw new GraphInvalidSizeException(width, height);

  unsigned            number_of_edges = CalculateNumberOfEdges(width, height);
  std::vector< Edge > edges(number_of_edges);

  for (unsigned i = 0; i < height; i++)
  {
    for (unsigned j = 0; j < width; j++)
    {
      unsigned node_pos = i * width + j;

      // if we can create a vertex going to the next node
      if (CanCreateNodeForward(j, width))
        edges.push_back({ .vertex_a = node_pos, .vertex_b = node_pos + 1 });

      // if we can create a vertex going to the node below us
      if (CanCreateNodeBottom(i, width, height))
        edges.push_back({ .vertex_a = node_pos, .vertex_b = (i + 1) * width + j });
    }
  }
  return edges;
}

inline void printGraph(unsigned width, unsigned height) noexcept(true) {
    unsigned int number_edges = CalculateNumberOfEdges(width, height);
    
    // Print top border
    std::cout << "+";
    for (unsigned i = 0; i < width; ++i) {
        std::cout << "--+";
    }
    std::cout << std::endl;

    // Print rows
    for (unsigned i = 0; i < height; ++i) {
        // Print nodes and vertical edges
        for (unsigned j = 0; j < width; ++j) {
            std::cout << "|  ";
        }
        std::cout << "|" << std::endl;

        // Print horizontal edges
        for (unsigned j = 0; j < width; ++j) {
            std::cout << "+--";
        }
        std::cout << "+" << std::endl;
    }
}

int main() {
    unsigned int width = 10;
    unsigned int height = 10;

    auto edges = GenerateBaseGraph(width, height);
    printGraph(width, height);

    return 0;
}