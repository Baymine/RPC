#include "./Stack.h"
#include <vector>

std::vector<int> elements; 

void Stack::push(int element) {
  elements.push_back(element); 
}

int Stack::pop() {
  int top = elements.back();
  elements.pop_back();
  return top;
}

int Stack::size() {
  return elements.size();
}