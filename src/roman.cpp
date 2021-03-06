#include "solution.cpp"
#include <fstream>

using namespace std;

class Roman {
public:
  vector<vector<bool>> adj;
  int size;
  int optimal;

  Roman(string file_name) { size = read_input(file_name); }

  int read_input(string file_name) {
    int edges, a, b;
    ifstream file(file_name);
    file >> optimal;
    file >> size;
    file >> edges;

    adj = vector<vector<bool>>(size, vector<bool>(size, false));
    for (int i = 0; i < edges; i++) {
      file >> a;
      file >> b;
      adj[a - 1][b - 1] = true;
      adj[b - 1][a - 1] = true;
    }
    return size;
  }

  int insertion_cost(int elem, const Solution sol) {
    if (sol.variables[elem] == 2)
      return 0;

    // custo de transformar em 2
    int soma = 2 - sol.variables[elem];
    for (int i = 0; i < size; i++)
      // para cada vizinho que é 1 e poderá virar 0
      if (adj[elem][i] and sol.variables[i] == 1)
        soma--;
    return soma;
  }

  int removal_cost(int elem, const Solution sol) {
    if (sol.variables[elem] != 2)
      return 0;

    // se ele já está protegido, eu economizo 2 (2 -> 0)
    // se ele ainda não está protegido, apenas 1 (2 -> 1)
    int soma;
    if (sol.protection[elem] > 0)
      soma = -2;
    else
      soma = -1;

    for (int i = 0; i < size; i++)
      if (adj[elem][i] and sol.protection[i] == 1)
        // cada vizinho que só tinha ele como proteção
        soma++;
    return soma;
  }

  int exchange_cost(int in, int out, const Solution sol) {
    if (in == out)
      return 0;
    if (sol.variables[in] == 2)
      return removal_cost(out, sol);
    if (sol.variables[out] != 2)
      return insertion_cost(in, sol);

    int soma = 0;
    soma += insertion_cost(in, sol);
    soma += removal_cost(out, sol);
    return soma;
  }

  void recheck(Solution *sol) {
    sol->protection = vector<int>(size, 0);
    for (int i = 0; i < size; i++)
      for (int j = i + 1; j < size; j++)
        if (adj[i][j]) {
          if (sol->variables[i] == 2)
            sol->protection[j]++;
          if (sol->variables[j] == 2)
            sol->protection[i]++;
        }
    for (int i = 0; i < size; i++)
      if (sol->variables[i] != 2) {
        if (sol->protection[i] > 0)
          sol->variables[i] = 0;
        else
          sol->variables[i] = 1;
      }
    sol->cost = 0;
    for (auto i : sol->variables)
      sol->cost += i;
  }
  void insert(int in, Solution *sol) {
    sol->variables[in] = 2;
    recheck(sol);
  }
  void remove(int out, Solution *sol) {
    sol->variables[out] = 0;
    recheck(sol);
  }
};
