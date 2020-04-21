#include <iostream>
#include <methods.h>
#include <future>

using namespace std;

static mutex mtx;
static condition_variable cv;
static atomic<int> counter = 0;

// Сумма элементов векторов
int make_sum(vector<int>& vec, size_t from, size_t to)
{
  auto x = 0;
  for (auto i = from; i < to; i++)
  {
    x += vec[i];
  }
  return x;
}

// Скалярное произведение векторов
int scalar_product(vector<vector<int>>& vectors, size_t from, size_t to)
{
  vector<int> results;
  int sum = 0;

  // Считаем произведение
  for (int i = from; i < to - 1; i++)
  {
    auto X1 = vectors[i][0];
    auto Y1 = vectors[i][1];
    auto X2 = vectors[i + 1][0];
    auto Y2 = vectors[i + 1][1];
    results.push_back(X1 * X2 + Y1 * Y2);
  }

  // Готовим поток к старту суммирования
  counter++;
  unique_lock<mutex> lck(mtx);
  cout << from << "..." << to << " is preparing..." << endl;

  // Когда все 10 потоков готовы - даём старт
  cv.wait(lck, [] { return counter == 10; });
  cout << from << "..." << to << " started!" << endl;

  // Суммируем элементы в каждом потоке
  for (int x : results)
  {
    sum += x;
  }
  cout << from << "..." << to << " sum " << sum << endl;
  return sum;
}

void Task1()
{
  cout << "Task 1:" << endl;
  vector<int> vec = Methods::generate_vector(1000);
  vector<future<int>> vectors;
  int result = 0;

  // Распределяем работу
  for (auto i = 0; i < 10; i++)
  {
    vectors.push_back(async(launch::async, make_sum, ref(vec), i * 100, i * 100 + 100));
  }

  // Суммируем результаты из потоков
  for (auto& v : vectors)
  {
    result += v.get();
  }
  cout << "Total result: " << result << endl << endl;
}

void Task23()
{
  cout << "Task 2:" << endl;
  vector<vector<int>> vectors = Methods::generate_algebraic_vectors(1000);
  vector<future<int>> results;
  int result = 0;

  // Распределяем работу
  for (auto i = 0; i < 10; i++)
  {
    results.push_back(async(launch::async, scalar_product, ref(vectors), i * 100, i * 100 + 100));
  }

  // Суммируем результаты потоков
  for (auto& r : results)
  {
    result += r.get();
  }
  cout << "Total sum: " << result;
}

int main()
{
  Task1();
  Task23();
}
