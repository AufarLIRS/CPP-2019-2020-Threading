#ifndef METHODS_H
#define METHODS_H

#include <vector>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

class Methods
{
public:
  static vector<int> generate_vector(size_t size);

  static vector<std::vector<int>> generate_algebraic_vectors(size_t size);
};

#endif  // METHODS_H
