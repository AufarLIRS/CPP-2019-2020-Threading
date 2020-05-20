#include <iostream>
#include <vector>
#include <thread>

void task2();
void getVectors(std::vector<int>& a, std::vector<int>& b);
void fakeGetVectors(std::vector<int>& a, std::vector<int>& b);
std::vector<int> calc(std::vector<int>& a, std::vector<int>& b);

void task2()
{
  std::vector<int> a;
  std::vector<int> b;
  // getVectors(a, b);
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
        result[j] = b.at(j) * a.at(j);
    });
  }

  return result;
}

void fakeGetVectors(std::vector<int>& a, std::vector<int>& b)
{
  for (int i = 0; i < 10000; i++)
  {
    a.push_back(2);
  }
  std::cout << "input vector B" << std::endl;
  for (int i = 0; i < 10000; i++)
  {
    b.push_back(2);
  }
}
