
#include <iostream>
#include <vector>
#include <thread>

void task1();
std::vector<int>& setupVector(std::vector<int>& vec, int size);
long long int count(std::vector<int>& vec, int threadNum);

void task1()
{
  std::vector<int> vec;
  setupVector(vec, 1000);
  long long int sum = count(vec, 10);
  std::cout << sum << std::endl;
}

std::vector<int>& setupVector(std::vector<int>& vec, int size)
{
  for (int i = 0; i < size; i++)
  {
    vec.push_back(i + 1);
  }
}

long long int count(std::vector<int>& vec, int threadNum)
{
  if (vec.size() < threadNum)
    threadNum = vec.size();
  int partLength = vec.size() / threadNum;
  if (vec.size() % threadNum)
    threadNum++;

  int* results = new int[threadNum];
  for (int i = 0; i < threadNum; i++)
    results[i] = 0;
  std::thread** threads = new std::thread*[threadNum];
  for (int i = 0; i++; i < threadNum)
  {
    threads[i] = new std::thread([&]() {
      for (int j = 0; i * partLength + j < vec.size() && j < partLength; j++)
        results[i] += vec.at(j);

    });
  }

  for (int i = 0; i < threadNum; i++)
    threads[i]->join();

  long long int result = 0;
  for (int i = 0; i < threadNum; i++)
    result += results[i];

  delete[] results;
  delete[] threads;  // is this correct?
  return result;
}
