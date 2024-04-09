#pragma once
#include "../../include/graph/graph.h"
#include <iostream>

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
  for (unsigned i = 0;i<h;i++) {
    for (unsigned j = 0;j<w;j++) {
      data.push_back(Token{.item=NO_ITEM, .value=0});
    }
  }
  
}

std::vector<Token> Graph::get_data() noexcept(false) {
  return data;
}

Graph::Graph(unsigned width, unsigned height) noexcept(false)
{
  if (width == 0 || height == 0) throw new GraphInvalidSizeException(width, height);

  w = width;
  h = height;
}

void printGraph(unsigned width, unsigned height) noexcept(true) {
    for (unsigned i = 0; i < height; ++i) {
        // Print horizontal edges
        for (unsigned j = 0; j < width; ++j) {
            std::cout << "+--";
        }
        std::cout << "+" << std::endl;

        // Print nodes and vertical edges
        for (unsigned j = 0; j < width; ++j) {
            // Print node
            std::cout << "|  ";
        }
        std::cout << "|" << std::endl;
    }
}