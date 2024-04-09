#pragma once

#include <exception>
#include <string>

static const std::string default_graph_message
  = "Base graph exception, something wrong happened surrounding the graph";

class GraphException : public std::exception
{
    private:
  std::string message = default_graph_message;

    public:
  GraphException(std::string detail = default_graph_message) : message(detail) {}

  std::string what() { return message; }
};

class GraphInvalidSizeException : public GraphException
{
    private:
  unsigned w;
  unsigned h;

    public:
  GraphInvalidSizeException(unsigned width, unsigned height) : w(width), h(height) {}

  std::string what()
  {
    return std::to_string(w) + "x" + std::to_string(h) + " is not a valid graph size!";
  }
};
