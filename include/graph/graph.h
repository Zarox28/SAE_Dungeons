#pragma once

#include "../../include/graph/exceptions.h"

#include <stack>
#include <vector>

enum Type
{
  /// The two cells cannot be connected, most likely they are not neighbors
  UNREACHABLE,
  /// There's a wall in between these two cells
  WALLED,
  /// The two cells are connected but nothing is on the path between them
  NO_ITEM,
  /// The two cells are connected and there is a monster on the path
  MONSTER,
  /// The two cells are connected and there is a health pack on the path
  HEALTH_PACK,
  /// The two cells are connected and there is treasure on the path
  TREASURE
};

struct Connection
{
  /// The state of the connection
  Type type;
  /**
   * Any integer value related to the type of the connection,
   * might be health gainned or losed, treasure increase
   */
  int value;
};

struct DijkstraConfig {
    float weight_health = 1;
    float weight_money = 1;
};

class Graph
{
    private:
  /**
   * The number of columns the graph has
   *
   * Note: The user must verify the validity of the value before manipulating
   * this value, if you are unable to do so use public setter/getter
   * `Graph::get_width()` and `Graph::set_width()`
   */
  unsigned w;

  /**
   * The number of rows the graph has
   *
   * Note: The user must verify the validity of the value before manipulating
   * this value, if you are unable to do so use public setter/getter
   * `Graph::get_height()` and `Graph::set_height()`
   */
  unsigned h;

  /**
   * Internal storage of the graph
   *
   * NOTE: This shouldn't be use directly unless strictly necessary instead use the
   * conveniance accessors `Graph::set/get_connection_between()`
   */
  std::vector< Connection > data;

    public:
  /// Returns the number of elements per line in the graph
  unsigned get_witdh() noexcept;

  /// Returns the number lines in the graph
  unsigned get_height() noexcept;

  /**
   * Returns the connection between two cells
   *
   * @param node_a The cell to check the connection from
   * @param node_b The cell to check the connection to
   *
   * @return connection between two cells
   */
  Connection get_connection_between(unsigned node_a, unsigned node_b) noexcept;

  /**
   * Sets the width of the graph
   *
   * @param width The new width of the graph
   * @throws GraphInvalidSizeException is width is 0
   */
  void set_width(unsigned width) noexcept(false);

  /**
   * Sets the height of the graph
   *
   * @param width The new height of the graph
   * @throws GraphInvalidSizeException is height is 0
   */
  void set_height(unsigned height) noexcept(false);

  /**
   * Sets the connection between two cells,
   * Doesn't not check for out of bounds access
   *
   * @param node_a The cell to set the connection from
   * @param node_b The cell to set the connection to
   */
  void set_connection_between(
    unsigned node_a, unsigned node_b, Connection connection
  ) noexcept;

  /**
   * Scrambles the graph using a random DFS algorithm
   *
   * @param start_node Where to start scrambling from
   */
  void Scramble(unsigned start_node, unsigned exit_node) noexcept;

  unsigned argMin(std::vector< unsigned int > distances, std::vector< bool > visited);
  std::vector< unsigned int > dijkstra(int start, int end, Graph* g, DijkstraConfig config);

  /**
   * Removes a given percentage of walls randomly
   *
   * @param percentage Percentage of walls to remove
   */
  void RemoveRandomWalls(uint percentage) noexcept;

  /**
   * Places items on the graph
   */
  void PlaceItems() noexcept;

  /**
   * Prints the adjacency matrix for the graph, where 1 represents a wall and 0
   * something else
   */
  void PrintAjacencyMatrix();
  std::vector<Connection>* GetData();

  /// Creates a basic graph where all possible walls exists
  void GenerateBaseGraph() noexcept;
  void Suicide() noexcept;

  /**
   * Initializes a graph with provided width and height
   * This does not create a usable graph, use this in combination with `Scramble()` to get
   * a working graphh
   *
   * @param width How many nodes per line to initialize the graph with
   * @param height How many lines to initialize the graph with
   *
   * @throws GraphInvalidSizeException if width or height is 0
   */
  Graph(unsigned width, unsigned height) noexcept(false);
};
