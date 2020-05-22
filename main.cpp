#include <iostream>
#include <future>
#include <algorithm>
#include <cstdlib>
#include <vector>

using namespace std;

static condition_variable cv;

// Task 1
int vector_sum(vector<int>& v, size_t from, size_t to)
{
  int sum = 0;
  for (auto i = from; i < to; i++)
  {
    sum = sum + v[i];
  }
  return sum;
}

void Task1()
{
  vector<int> vector1(1000);
  vector<future<int>> result_vector;
  generate(vector1.begin(), vector1.end(), rand);
  int result = 0;

  for (auto i = 0; i < 10; i++)
  {
    result_vector.push_back(async(launch::async, vector_sum, ref(vector1), i * 100, i * 100 + 100));
  }

  for (auto& v : result_vector)
  {
    result = result + v.get();
  }

  cout << "Task 1: sum = " << result << endl << endl;
}

// Task 2
int product(vector<int>& v1, vector<int>& v2, size_t from, size_t to)
{
  static mutex mtx;
  static atomic<int> a = 0;
  unique_lock<mutex> m_lck(mtx);
  static array<int, 1000> arr;
  int result = 0;

  for (auto i = from; i < to; i++)
  {
    arr[i] = v1[i] * v2[i];
  }

  a++;
  cout << "Preparing: " << from << "-" << to << endl;
  cv.wait(m_lck, [] { return a == 10; });
  cout << "Started: " << from << "-" << to << endl;

  for (auto i = from; i < to; i++)
  {
    result = result + arr[i];
  }
  return result;
}

void Task2()
{
  vector<int> vector1(1000);
  vector<int> vector2(1000);

  generate(vector1.begin(), vector1.end(), rand);
  generate(vector2.begin(), vector2.end(), rand);

  vector<future<int>> result_vector;

  for (auto i = 0; i < 1000; i += 100)
  {
    result_vector.push_back(async(launch::async, product, ref(vector1), ref(vector2), i, i + 100));
  }

  int result = 0;
  for (auto& i : result_vector)
  {
    result = result + i.get();
  }

  cout << "Task 2: result = " << result << endl << endl;
}

// Task 3
void Task3()
{
  vector<int> vector1(1000);
  vector<int> vector2(1000);

  generate(vector1.begin(), vector1.end(), rand);
  generate(vector2.begin(), vector2.end(), rand);

  vector<future<int>> result_vector;

  for (auto i = 0; i < 1000; i += 100)
  {
    result_vector.push_back(async(launch::async, product, ref(vector1), ref(vector2), i, i + 100));
  }

  cv.notify_all();

  int result = 0;
  for (auto& i : result_vector)
  {
    result = result + i.get();
  }
  cout << "Task 3: result = " << result << endl;
}

int main()
{
  Task1();
  Task2();
  Task3();
  return 0;
}
