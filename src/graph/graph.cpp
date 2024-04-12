#pragma once
#include "../../include/graph/graph.h"
#include <iostream>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <random>

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
  data.resize(h * w, Token { .item = DISCONNECTED, .value = 0 });

  for (unsigned row = 0; row < h; row++)
  {
    for (unsigned col = 0; col < w; col++)
    {
      unsigned node_num = (row * w + col);

      if (col + 1 < w)
        data[node_num * w + (node_num + 1)] = Token { .item = NO_ITEM, .value = 0 };

      if (row + 1 < h)
        data[node_num * w + (node_num + w)] = Token { .item = NO_ITEM, .value = 0 };
    }
  }
}

std::vector< Token > Graph::get_data() noexcept(false) { return data; }

// pretty sure this doesn't work as intended
void Graph::scramble(unsigned start_node) noexcept
{
  std::vector< bool > visited(w);
  visited[start_node] = true;

  std::vector< unsigned > stack;
  stack.push_back(start_node);

  auto rng = std::default_random_engine {};
  while (! stack.empty())
  {
    unsigned node = stack.back();  // ref
    stack.pop_back();

    unsigned pos_in_matrix_data = w * node;

    std::vector< int > neighbours(4);

    for (int i = 0; i < 4; i++) neighbours.push_back(pos_in_matrix_data + i);

    std::shuffle(neighbours.begin(), neighbours.end(), rng);
    for (size_t i = 0; i < neighbours.size(); i++)
    {
      if (visited[i]) continue;
      data[pos_in_matrix_data + i] = { .item = NO_ITEM, .value = 0 };
      visited[i]                   = true;
      stack.push_back(i);
    }
  }
}

Graph::Graph(unsigned width, unsigned height) noexcept(false)
{
  if (width == 0 || height == 0) throw new GraphInvalidSizeException(width, height);

  w = width;
  h = height;
}

void Graph::DebugGraph()
{
  printf("----DEBUG\n");
  for (unsigned i = 0; i < h; i++)
  {
    for (unsigned j = 0; j < w; j++)
    {
      Token current = data[i * w + j];
      if (current.item != DISCONNECTED)
        printf("1");
      else
        printf("0");
    }
    printf("\n");
  }
  printf("----DEBUG");
}

