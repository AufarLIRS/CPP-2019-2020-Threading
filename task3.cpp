#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <algorithm>
#include <mutex>

namespace task3
{
std::mutex mutex_;
std::condition_variable cv;

void task3();
void getVectors(std::vector<int>& a, std::vector<int>& b);
std::vector<int> calc(std::vector<int>& a, std::vector<int>& b);

void task3()
{
  std::vector<int> a(10000);
  generate(a.begin(), a.end(), rand);
  std::vector<int> b(10000);
  generate(b.begin(), b.end(), rand);
  std::vector<int> result = calc(a, b);
  int scalar_mult = 0;
  for (int i = 0; i < result.size(); i++)
  {
    scalar_mult += result[i];
  }
  std::cout << scalar_mult << std::endl;
}

void getVectors(std::vector<int>& a, std::vector<int>& b)
{
  int size;
  std::cin >> size;
  while (size <= 0)
  {
    std::cout << "invalid size" << std::endl;
    std::cin >> size;
  }

  int tmp;
  std::cout << "input vector A" << std::endl;
  for (int i = 0; i < size; i++)
  {
    std::cin >> tmp;
    a.push_back(tmp);
  }
  std::cout << "input vector B" << std::endl;
  for (int i = 0; i < size; i++)
  {
    std::cin >> tmp;
    b.push_back(tmp);
  }
}

std::vector<int> calc(std::vector<int>& a, std::vector<int>& b)
{
  std::vector<int> result;
  int threadNum = 100;

  if (a.size() < threadNum)
    threadNum = a.size();

  int partLength = a.size() / threadNum;

  if (a.size() % threadNum)
    threadNum++;

  std::thread** threads = new std::thread*[threadNum];

  for (int i = 0; i < a.size(); i++)
  {
    result.push_back(0);
  }
  for (int i = 0; i++; i < threadNum)
  {
    threads[i] = new std::thread([&]() {
      for (int j = 0; i * partLength + j < a.size() && j < partLength; j++)
      {
        std::lock_guard<std::mutex> lk(mutex_);
        result[j] = b.at(j) * a.at(j);
      }

      {
        std::lock_guard<std::mutex> lk(mutex_);
        threadNum--;
      }
      cv.notify_all();
    });
  }

  while (threadNum > 0)
    wait();

  return result;
}

}  // namespace task3
