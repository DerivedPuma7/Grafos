#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main() {
  priority_queue<int, vector<int>, greater<int>> minHeap;

  minHeap.push(10);
  minHeap.push(20);
  minHeap.push(5);
  minHeap.push(7);

  while (!minHeap.empty()) {
    cout << minHeap.top() << " ";
    minHeap.pop();
  }
  return 0;
}
