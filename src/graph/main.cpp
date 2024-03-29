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

inline bool
CanCreateNodeBottom(unsigned line_number, unsigned element_per_line, unsigned grid_height)
{
  return (line_number + 1) * element_per_line < grid_height;
}

extern std::vector< Edge >
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
	std::vector<std::vector<int>> matrice(width, std::vector<int>(height));
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; i++) {
			std::cout << " - " << std::endl;
		}
	}
}

/*
* --- Latest ---
*
* Author : Shrayzz
* Time : 29/03/2024 13:30
*
* Chengelog :
* > Added "#include <iostream> for terminal printing
* > Doing some tests for print matrice with width and height dimensions
* >
* > Added : Line(s) 52 - 60
*/