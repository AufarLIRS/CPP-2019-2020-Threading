#include <iostream>
#include <methods.h>
#include <future>

using namespace std;

static mutex mtx;
static condition_variable cv;
static atomic<int> counter = 0;
static vector<int> vector1;  // Task 2
static vector<int> vector2;  // Task 3

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

// Скалярное произведение векторов (Task 2)
void scalar_product(vector<int>& vec1, vector<int>& vec2, size_t from, size_t to)
{
  // Считаем произведение, складываем в общий вектор
  for (int i = from; i < to; i++)
  {
    vector1.push_back(vec1[i] * vec2[i]);
  }
}

// Скалярное произведение векторов и сложение (Task 3)
int scalar_product_and_sum(vector<int>& vec1, vector<int>& vec2, size_t from, size_t to)
{
  int sum = 0;

  // Считаем произведение
  for (int i = from; i < to; i++)
  {
    vector2.push_back(vec1[i] * vec2[i]);
  }

  // Готовим поток к старту суммирования
  counter++;
  unique_lock<mutex> lck(mtx);
  cout << from << "..." << to << " is preparing..." << endl;

  // Когда все 10 потоков готовы - даём старт
  cv.wait(lck, [] { return counter == 10; });
  cout << from << "..." << to << " started!" << endl;

  // Суммируем элементы в каждом потоке
  for (int i = from; i < to; i++)
  {
    sum += vector2[i];
  }

  cout << from << "..." << to << " sum is " << sum << endl;
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

void Task2()
{
  cout << "Task 2:" << endl;
  vector<int> vec1 = Methods::generate_vector(1000);
  vector<int> vec2 = Methods::generate_vector(1000);

  int result = 0;

  // Распределяем работу
  for (auto i = 0; i < 10; i++)
  {
    async(launch::async, scalar_product, ref(vec1), ref(vec2), i * 100, i * 100 + 100);
  }

  // Суммируем результаты из общего вектора
  for (auto& v : vector1)
  {
    result += v;
  }
  cout << "Total sum: " << result << endl << endl;
}

void Task3()
{
  cout << "Task 3:" << endl;
  vector<int> vec1 = Methods::generate_vector(1000);
  vector<int> vec2 = Methods::generate_vector(1000);
  vector<future<int>> results;

  int result = 0;

  // Распределяем работу
  for (auto i = 0; i < 10; i++)
  {
    results.push_back(async(launch::async, scalar_product_and_sum, ref(vec1), ref(vec2), i * 100, i * 100 + 100));
  }

  cv.notify_all();

  // Суммируем результаты из потоков
  for (auto& r : results)
  {
    result += r.get();
  }

  cout << "Total sum: " << result;
}

int main()
{
  Task1();
  Task2();
  Task3();
}
