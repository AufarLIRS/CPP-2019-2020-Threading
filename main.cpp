#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std;

static mutex mtx;
static condition_variable cv;

static vector<thread> threads;

static vector<int> v1;
static vector<int> v2;
static vector<int> global;

static array<int, 1000> save;

static atomic<int> finalsum = 0;
static atomic<int> counter = 0;

void clearall()
{
  threads.clear();
  v1.clear();
  v2.clear();
  finalsum = 0;
}

void partialSum(const vector<int>& vector, size_t from, size_t to)
{
  auto sum = 0;
  for (auto i = from; i < to; i++)
  {
    sum += vector.at(i);
  }
  unique_lock<mutex> lck(mtx);
  global.push_back(sum);
}

void Task1()
{
  clearall();

  for (auto i = 0; i < 1000; i++)
  {
    v1.push_back(i);
  }

  for (auto i = 0; i < 1000; i += 100)
  {
    threads.push_back(thread(partialSum, ref(v1), i, i + 99));
  }
  for (auto& t : threads)
    t.join();

  for (auto& e : global)
  {
    finalsum += e;
  }
  cout << "Task 1 sum: " << finalsum << endl;
}

void ScalarMul(const vector<int>& v1, const vector<int>& v2, size_t from, size_t to)
{
  for (auto i = from; i < to; i++)
  {
    save[i] = (v1.at(i) * v2.at(i));
  }

  counter++;

  unique_lock<mutex> lck(mtx);
  cout << "wait " << from << endl;
  cv.wait(lck, [] { return counter == 10; });
  cout << "go " << from << endl;

  auto localsum = 0;
  for (auto i = from; i < to; i++)
  {
    localsum += save[i];
  }
  finalsum += localsum;
}

void Task2and3()
{
  clearall();

  for (auto i = 0; i < 1000; i++)
  {
    v1.push_back(i);
    v2.push_back(i);
  }

  for (auto i = 0; i < 1000; i += 100)
  {
    threads.push_back(thread(ScalarMul, ref(v1), ref(v2), i, i + 100));
  }

  cv.notify_all();

  for (auto& t : threads)
    t.join();

  cout << "Task 2 and 3 sum: " << finalsum << endl;
}

int main()
{
  Task1();
  Task2and3();
  return 0;
}
