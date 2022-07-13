#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
  vector<int> variables;
  // protection[i] é a quantidade de adjacentes que estão te protegendo
  // não considera sua própria proteção
  vector<int> protection;
  int cost;

  Solution() { cost = 0; }

  Solution(int size) {
    variables = vector<int>(size, 1);
    protection = vector<int>(size, 1);
    cost = size;
  }

  Solution(const Solution &s) {
    variables = vector<int>(s.variables);
    protection = vector<int>(s.protection);
    cost = s.cost;
  }

  void print() {
    cout << "Solution\t cost = " << cost << " ";
    // for (auto i : variables)
    //   cout << i << " ";
    cout << endl;
  }
};
