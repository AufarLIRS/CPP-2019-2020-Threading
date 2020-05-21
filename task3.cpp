#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <algorithm>
#include <mutex>
#include <future>

namespace task3
{
std::mutex mutex_;
std::condition_variable cv;

void task3();
void getVectors(std::vector<int>& a, std::vector<int>& b);
long long int calc(std::vector<int>& a, std::vector<int>& b);

void task3()
{
  std::vector<int> a(10000);
  generate(a.begin(), a.end(), rand);
  std::vector<int> b(10000);
  generate(b.begin(), b.end(), rand);

  std::cout << calc(a, b) << std::endl;
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

long long int calc(std::vector<int>& a, std::vector<int>& b)
{
  std::vector<int> result;
  long long int result_sum = 0;
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
  int threadActive = threadNum;
  for (int i = 0; i++; i < threadNum)
  {
    threads[i] = new std::thread(std::async([&]() {
      for (int j = 0; i * partLength + j < a.size() && j < partLength; j++)
        result[j] = b.at(j) * a.at(j);

      threadActive--;
      std::unique_lock<std::mutex> lock(mutex_);
      cv.wait(lock, [=] { return threadActive == 0; });
      for (int j = 0; i * partLength + j < a.size() && j < partLength; j++)
      {
        std::unique_lock<std::mutex> lock(mutex_);
        result_sum += result[i];
      }
    }));
  }

  cv.notify_all();

  return result_sum;
}

}  // namespace task3
