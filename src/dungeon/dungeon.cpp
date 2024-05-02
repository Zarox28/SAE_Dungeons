#pragma once

#include "../../include/dungeon/dungeon.h"

#include <iostream>

// Modified from Corentin S code
void Dungeon::RenderAsciiFrame()
{
  const unsigned size = this->graph.get_height();

  std::vector< std::vector< char > > GrilleAffichageDonjon(size * 2 + 1, std::vector< char >(size * 2 + 1, '-'));

  int x, y;
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      x                           = i * 2 + 1;
      y                           = j * 2 + 1;
      GrilleAffichageDonjon[x][y] = ' ';
    }
  }

  // placement des croix dans les coins

  for (int i = 0; i < size * 2 + 1; i++)
  {
    for (int j = 0; j < size * 2 + 1; j++)
      if (i % 2 == 0 && j % 2 == 0) GrilleAffichageDonjon[i][j] = '+';
  }

  // placement des murs
  for (int i = 0; i < size * 2 + 1; i++)
  {
    for (int j = 0; j < size * 2 + 1; j++)
      if (i % 2 == 1 && j % 2 == 0) GrilleAffichageDonjon[i][j] = '|';
  }

  // Destruction des murs

  int pointEnVerif, x1, y1, pointEnConnexion;

  for (int i = 0; i < size * size; i++)
  {
    for (int k = 0; k < size * size; k++)
    {
      pointEnVerif = k;
      for (int j = 0; j < 4; j++)
      {
        switch (j)
        {
          case (0) :
            x                = pointEnVerif / size;
            y                = pointEnVerif % size;
            x1               = x - 1;
            y1               = y;
            pointEnConnexion = x1 * size + y1;
            if (x - 1 >= 0)
            {
              if (graph.get_connection_between(x, y).type != WALLED && graph.get_connection_between(x, y).type != UNREACHABLE)
              {
                GrilleAffichageDonjon[x * 2][y * 2 + 1] = ' ';
              }
            }
            break;
          case (1) :
            x                = pointEnVerif / size;
            y                = pointEnVerif % size;
            x1               = x;
            y1               = y + 1;
            pointEnConnexion = x1 * size + y1;
            if (y + 1 < size)
            {
              if (graph.get_connection_between(x, y).type != WALLED && graph.get_connection_between(x, y).type != UNREACHABLE)
              {
                GrilleAffichageDonjon[x * 2 + 1][y * 2 + 2] = ' ';
              }
            }
            break;
          case (2) :
            x                = pointEnVerif / size;
            y                = pointEnVerif % size;
            x1               = x + 1;
            y1               = y;
            pointEnConnexion = x1 * size + y1;
            if (x + 1 < size)
            {
              if (graph.get_connection_between(x, y).type != WALLED && graph.get_connection_between(x, y).type != UNREACHABLE)
              {
                GrilleAffichageDonjon[x * 2 + 2][y * 2 + 1] = ' ';
              }
            }
            break;
          case (3) :
            x                = pointEnVerif / size;
            y                = pointEnVerif % size;
            x1               = x;
            y1               = y - 1;
            pointEnConnexion = x1 * size + y1;
            if (y - 1 >= 0)
            {
              if (graph.get_connection_between(x, y).type != WALLED && graph.get_connection_between(x, y).type != UNREACHABLE)
              {
                GrilleAffichageDonjon[x * 2 + 1][y * 2] = ' ';
              }
            }
            break;
        }
      }
    }
  }

  for (int i = 0; i < size * 2 + 1; i++)
  {
    for (int j = 0; j < size * 2 + 1; j++) std::cout << GrilleAffichageDonjon[i][j] << " ";
    std::cout << std::endl;
  }
}

Graph* Dungeon::get_graph() noexcept {
    return &graph;
}

Dungeon::Dungeon(unsigned width, unsigned height, unsigned start_x, unsigned start_y) noexcept(false) :
  graph(width, height)
{
  if (start_x > width - 1 || start_y > height - 1) throw new DungeonException("Invalid start position");

  start_cell = start_y * width + start_x;

  graph.GenerateBaseGraph();
  graph.Scramble(start_cell);
  graph.Scramble(start_cell);
  // graph.PrintAjacencyMatrix();
}
