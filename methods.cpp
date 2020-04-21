#include <methods.h>

// Генератор элементов в векторе
vector<int> Methods::generate_vector(size_t size)
{
  srand(time(0));
  vector<int> vec;
  for (int i = 0; i < size; i++)
  {
    vec.push_back(rand() % 9 + 1);
  }
  return vec;
}

// Генератор двумерных векторов в общем векторе
vector<vector<int>> Methods::generate_algebraic_vectors(size_t size)
{
  srand(time(0));
  vector<vector<int>> vectors(size);
  for (int i = 0; i < size; i++)
  {
    vectors[i].push_back(rand() % 9 + 1);  // X
    vectors[i].push_back(rand() % 9 + 1);  // Y
  }
  return vectors;
}
