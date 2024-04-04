#pragma once
#include "../../include/graph/graph.h"

unsigned Graph::CalculateNumberOfEdges(unsigned width, unsigned height)
{
  unsigned edges_per_line   = width - 1;
  unsigned edges_per_column = width;
  unsigned total_edges      = edges_per_line * height + edges_per_column * (height - 1);

  return total_edges;
}

bool Graph::CanCreateNodeForward(unsigned index_on_line, unsigned grid_width)
{
  return index_on_line + 1 < grid_width;
}

bool Graph::CanCreateNodeBottom(
  unsigned line_number, unsigned element_per_line, unsigned grid_height
)
{
  return (line_number + 1) * element_per_line < grid_height;
}

unsigned Graph::get_witdh() noexcept { return w; }

unsigned Graph::get_height() noexcept { return h; }

void Graph::set_width(unsigned width) noexcept(false)
{
  if (width == 0) throw new GraphInvalidSizeException(width, h);
  w = width;
}

void Graph::set_height(unsigned height) noexcept(false)
{
  if (height == 0) throw new GraphInvalidSizeException(w, height);
  h = height;
}

void Graph::GenerateBaseGraph() noexcept(false)
{
  unsigned            number_of_edges = CalculateNumberOfEdges(w, h);
  std::vector< Edge > edges(number_of_edges);

  for (unsigned i = 0; i < h; i++)
  {
    for (unsigned j = 0; j < w; j++)
    {
      unsigned node_pos = i * w + j;

      // if we can create a vertex going to the next node
      if (CanCreateNodeForward(j, w))
        edges.push_back({ .vertex_a = node_pos, .vertex_b = node_pos + 1 });

      // if we can create a vertex going to the node below us
      if (CanCreateNodeBottom(i, w, h))
        edges.push_back({ .vertex_a = node_pos, .vertex_b = (i + 1) * w + j });
    }
  }
  data = edges;
}

std::vector<Edge> Graph::get_data() noexcept(false) {
  return data;
}

Graph::Graph(unsigned width, unsigned height) noexcept(false)
{
  if (width == 0 || height == 0) throw new GraphInvalidSizeException(width, height);

  w = width;
  h = height;
}
