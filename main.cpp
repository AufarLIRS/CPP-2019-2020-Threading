#include <iostream>
#include <future>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <numeric>
#include <future>

using namespace std;

static mutex mtx;
static condition_variable cv;
static atomic<int> i = 0;
static array<int, 1000> temp;

int sum(vector<int>& vec, size_t from, size_t to)
{
  auto x = 0;
  for (auto i = from; i < to; i++)
  {
    x += vec[i];
  }
  return x;
}

int scalar(const vector<int>& v1, const vector<int>& v2, size_t from, size_t to)
{
  for (auto i = from; i < to; i++)
  {
    temp[i] = (v1.at(i) * v2.at(i));
  }
  i++;
  unique_lock<mutex> lck(mtx);
  cout << "wait " << from << endl;
  cv.wait(lck, [] { return i == 10; });
  cout << "go " << from << endl;

  auto arr_sum = 0;
  for (auto i = from; i < to; i++)
  {
    arr_sum += temp[i];
  }
  return arr_sum;
}

//------------------------------------------------------------------------------------------------------//
//-----------------------------------------Task 1-------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
void Task1()
{
  vector<int> vec(1000);
  vector<future<int>> task1;
  generate(vec.begin(), vec.end(), rand);
  int result = 0;

  for (auto i = 0; i < 10; i++)
  {
    task1.push_back(async(launch::async, sum, ref(vec), i * 100, i * 100 + 100));
  }

  for (auto& v : task1)
  {
    result += v.get();
  }
  cout << "Task 1: " << result << endl << endl;
}

//------------------------------------------------------------------------------------------------------//
//-----------------------------------------Task 2-------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
void Task2()
{
  vector<int> v1(1000);
  vector<int> v2(1000);

  generate(v1.begin(), v1.end(), rand);
  generate(v2.begin(), v2.end(), rand);

  vector<future<int>> task2;

  for (auto i = 0; i < 1000; i += 100)
  {
    task2.push_back(async(launch::async, scalar, ref(v1), ref(v2), i, i + 100));
  }

  int result = 0;
  for (auto& i : task2)
  {
    result += i.get();
  }
  cout << "Task 2: " << result << endl;
}

//------------------------------------------------------------------------------------------------------//
//-----------------------------------------Task 3-------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//

void Task3()
{
  vector<int> v1(1000);
  vector<int> v2(1000);

  generate(v1.begin(), v1.end(), rand);
  generate(v2.begin(), v2.end(), rand);

  vector<future<int>> task3;

  for (auto i = 0; i < 1000; i += 100)
  {
    task3.push_back(async(launch::async, scalar, ref(v1), ref(v2), i, i + 100));
  }

  cv.notify_all();

  int result = 0;
  for (auto& i : task3)
  {
    result += i.get();
  }
  cout << "Task 3: " << result << endl;
}

int main()
{
  Task1();
  Task2();
  Task3();
}
