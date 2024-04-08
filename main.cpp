#include "src/dungeon/dungeon.cpp"
#include <iostream>

int main()
{
  Dungeon* test_dungeon = new Dungeon(6, 6);
  std::cout << test_dungeon->RenderAsciiFrame();
}
