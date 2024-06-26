#include "../../include/graph/graph.h"

#include <algorithm>
#include <limits>
#include <random>
#include <vector>

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

void Graph::GenerateBaseGraph() noexcept
{
  const unsigned number_of_nodes = (h * w);
  data.resize(number_of_nodes * number_of_nodes, Connection { .type = UNREACHABLE, .value = 0 });

  for (unsigned row = 0; row < h; row++)
  {
    for (unsigned col = 0; col < w; col++)
    {
      unsigned current_node = row * w + col;
      if (col + 1 < w)
      {
        // Connect to the node on the right
        unsigned next_on_line = current_node + 1;
        set_connection_between(current_node, next_on_line, { .type = WALLED, .value = 0 });
        set_connection_between(next_on_line, current_node, { .type = WALLED, .value = 0 });
      }
      if (row + 1 < h)
      {
        // Connect to the node below
        unsigned below = current_node + w;
        set_connection_between(current_node, below, { .type = WALLED, .value = 0 });
        set_connection_between(below, current_node, { .type = WALLED, .value = 0 });
      }
    }
  }
}

void Graph::Scramble(unsigned start_node, unsigned exit_node) noexcept
{
  const unsigned number_of_nodes = w * h;

  std::random_device rand;
  std::mt19937       gen(rand());

  std::vector< bool >     visited(number_of_nodes, false);
  std::vector< unsigned > stack;

  visited[start_node] = true;
  stack.push_back(start_node);

  unsigned exit;
  unsigned furthest = 0;

  while (! stack.empty())
  {
    unsigned cell = stack.back();
    stack.pop_back();

    if (cell == exit_node) continue;

    std::vector< unsigned > neighbors;
    for (unsigned node = 0; node < number_of_nodes; node++)
      if (get_connection_between(cell, node).type == WALLED) neighbors.push_back(node);

    std::shuffle(neighbors.begin(), neighbors.end(), gen);

    for (unsigned neighbor : neighbors)
    {
      if (visited[neighbor]) continue;

      set_connection_between(cell, neighbor, { .type = NO_ITEM, .value = 1 });
      set_connection_between(neighbor, cell, { .type = NO_ITEM, .value = 1 });

      visited[neighbor] = true;
      stack.push_back(neighbor);
    }
  }
}

void Graph::RemoveRandomWalls(unsigned percentage) noexcept
{
  std::random_device                        rand;
  std::mt19937                              gen { rand() };
  std::uniform_int_distribution< unsigned > randomizer(0, 100);

  for (unsigned i = 0; i < data.size() - 1; i++)
  {
    if (data[i].type != WALLED) continue;
    if (randomizer(gen) > percentage) continue;

    data[i].type = NO_ITEM;
  }
}

void Graph::PlaceItems() noexcept
{
  std::random_device                        rand;
  std::mt19937                              gen { rand() };
  std::uniform_int_distribution< unsigned > randomizer(0, 100);
  std::uniform_int_distribution< unsigned > random_value(0, 20);

  for (unsigned i = 0; i < data.size() - 1; i++)
  {
    if (data[i].type != NO_ITEM) continue;
    if (randomizer(gen) > 10) continue;

    unsigned item_type = randomizer(gen) % 3;

    if (item_type == 0 && randomizer(gen) > 50)
      data[i].type = TREASURE;
    else if (item_type == 1)
      data[i].type = HEALTH_PACK;
    else
      data[i].type = MONSTER;

    data[i].value = random_value(gen);
  }
}

Graph::Graph(unsigned width, unsigned height) noexcept(false)
{
  if (width == 0 || height == 0) throw new GraphInvalidSizeException(width, height);

  w = width;
  h = height;
}

void Graph::PrintAjacencyMatrix()
{
  printf("----DEBUG\n");
  for (unsigned i = 0; i < h * w; i++)
  {
    for (unsigned j = 0; j < h * w; j++)
    {
      Connection current = get_connection_between(i, j);
      if (current.type == WALLED)
        printf("1");
      else
        printf("0");
    }
    printf("\n");
  }
  printf("----DEBUG\n");
}

Connection Graph::get_connection_between(unsigned node_a, unsigned node_b) noexcept
{
  return this->data[node_a * w * h + node_b];
}

void Graph::set_connection_between(unsigned node_a, unsigned node_b, Connection connection) noexcept
{
  this->data[node_a * w * h + node_b] = connection;
}

void Graph::Suicide() noexcept
{
  data.clear();
  delete this;
}

std::vector< Connection >* Graph::GetData() { return &data; }

unsigned Graph::argMin(std::vector< unsigned int > distances, std::vector< bool > visited)
{
  unsigned min   = std::numeric_limits< unsigned >::max();
  unsigned min_i = min;
  for (unsigned i = 0; i < distances.size(); i++)
  {
    if (visited[i]) continue;
    if (distances[i] > min) continue;
    min   = distances[i];
    min_i = i;
  }
  return min_i;
}

std::vector< unsigned int > Graph::dijkstra(int start, int end, Graph* g, DijkstraConfig config)
{
  const unsigned          number_of_nodes = g->get_height() * g->get_witdh();
  std::vector< unsigned > distances(number_of_nodes, std::numeric_limits< unsigned >::max());
  std::vector< unsigned > previous(number_of_nodes, 0);
  std::vector< bool >     visited(number_of_nodes, false);

  // d[x0] ← 0
  distances[start] = 0;

  for (unsigned i = 0; i < number_of_nodes; i++)
  {
    unsigned x = argMin(distances, visited);
    visited[x] = true;

    if (x == end) return previous;

    for (unsigned n = 0; n < number_of_nodes; n++)
    {
      Connection c = g->get_connection_between(x, n);
      if (visited[n] || c.type == UNREACHABLE || c.type == WALLED) continue;

      float weight = c.value;

      switch (c.type)
      {
        case MONSTER : weight *= config.weight_health; break;

        case HEALTH_PACK : weight *= 1 / config.weight_health; break;

        case TREASURE : weight *= 1 / config.weight_money; break;

        default : break;
      }

      if (distances[n] > (distances[x] + weight))
      {
        distances[n] = distances[x] + weight;
        previous[n]  = x;
      }
    }
  }
  return previous;
}
