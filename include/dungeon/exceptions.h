#pragma once

#include <exception>
#include <string>

static const std::string default_dungeon_message
  = "Dungeon exception, something wrong happened inside the dungeon !";

class DungeonException : public std::exception
{
    private:
  std::string message = default_dungeon_message;

    public:
  DungeonException(std::string detail = default_dungeon_message) : message(detail) {}

  std::string what() { return message; }
};

class DungeonInvalidHallwayLengthException : public DungeonException
{
    private:
  unsigned length;

    public:
  DungeonInvalidHallwayLengthException(unsigned hallway_length) : length(hallway_length)
  {}

  std::string what()
  {
    return std::to_string(length) + " is not a valid hallway length!";
  }
};
