#include "algorithm"
#include "chrono"
#include "iostream"
#include "roman.cpp"
#include <cstdint>
#include <cstdio>

class Grasp {
public:
  Solution sol;
  Roman obj_function;
  double alpha;
  int interations;
  int best;

  Grasp(double alpha, int interations, string file_name)
      : obj_function(file_name) {
    // BUG this se refere a essa instância, correto?
    this->alpha = alpha;
    this->interations = interations;
  }

  vector<int> make_cl() {
    vector<int> cl;
    for (int i = 0; i < obj_function.size; i++)
      if (sol.variables[i] != 2 and obj_function.insertion_cost(i, sol) < 0)
        cl.push_back(i);
    return cl;
  }

  void constructive_heuristic() {
    vector<int> cl = make_cl();
    vector<int> rcl;
    int cost = INT32_MAX;

    // Enquanto há melhorias
    while (cost > sol.cost) {
      int max_cost = INT32_MIN, min_cost = INT32_MAX;
      int delta_cost;
      cost = sol.cost;
      // update_cl??

      for (int c : cl) {
        delta_cost = obj_function.insertion_cost(c, sol);
        if (delta_cost < min_cost)
          min_cost = delta_cost;
        if (delta_cost > max_cost)
          max_cost = delta_cost;
      }

      for (int c : cl) {
        delta_cost = obj_function.insertion_cost(c, sol);
        if (delta_cost <= min_cost + alpha * (max_cost - min_cost))
          rcl.push_back(c);
      }

      if (rcl.empty())
        break;

      int rnd_index = rand() % rcl.size();
      int in_cand = rcl[rnd_index];
      cl.erase(remove(cl.begin(), cl.end(), in_cand), cl.end());
      obj_function.insert(in_cand, &sol);
      rcl.clear();
    }
  }

  void local_search() {
    if (best == 1)
      return local_search_best();
    return local_search_first();
  }
  void local_search_first() {
    int min_delta_cost, delta_cost;
    int cand_in = -1, cand_out = -1;
    vector<int> cl = make_cl();

    do {
      min_delta_cost = 0;

      for (int c : cl) {
        delta_cost = obj_function.insertion_cost(c, sol);
        if (delta_cost < min_delta_cost) {
          min_delta_cost = delta_cost;
          cand_in = c;
          cand_out = -1;
          break;
        }
      }
      if (cand_in != -1) {
        obj_function.insert(cand_in, &sol);
        cl.erase(remove(cl.begin(), cl.end(), cand_in), cl.end());
        continue;
      }

      for (int c : cl) {
        delta_cost = obj_function.removal_cost(c, sol);
        if (delta_cost < min_delta_cost) {
          min_delta_cost = delta_cost;
          cand_in = -1;
          cand_out = c;
          break;
        }
      }
      if (cand_out != -1) {
        obj_function.remove(cand_out, &sol);
        if (obj_function.insertion_cost(cand_out, sol) < 0)
          cl.push_back(cand_out);
        continue;
      }

      for (int c : cl) {
        for (int i = 0; i < obj_function.size; i++) {
          if (sol.variables[i] == 2) {
            delta_cost = obj_function.exchange_cost(c, i, sol);
            if (delta_cost < min_delta_cost) {
              min_delta_cost = delta_cost;
              cand_in = c;
              cand_out = i;
              break;
            }
          }
        }
      }

      if (min_delta_cost < 0) {
        if (cand_out != -1) {
          obj_function.remove(cand_out, &sol);
          if (obj_function.insertion_cost(cand_out, sol) < 0)
            cl.push_back(cand_out);
        } else {
          obj_function.insert(cand_in, &sol);
          cl.erase(remove(cl.begin(), cl.end(), cand_in), cl.end());
        }
      }
    } while (min_delta_cost < 0);
  }
  void local_search_best() {
    int min_delta_cost, delta_cost;
    int cand_in = -1, cand_out = -1;
    vector<int> cl = make_cl();

    do {
      min_delta_cost = 0;

      for (int c : cl) {
        delta_cost = obj_function.insertion_cost(c, sol);
        if (delta_cost < min_delta_cost) {
          min_delta_cost = delta_cost;
          cand_in = c;
          cand_out = -1;
        }
      }

      for (int c : cl) {
        delta_cost = obj_function.removal_cost(c, sol);
        if (delta_cost < min_delta_cost) {
          min_delta_cost = delta_cost;
          cand_in = -1;
          cand_out = c;
        }
      }

      // TODO exchange
      for (int c : cl) {
        for (int i = 0; i < obj_function.size; i++) {
          if (sol.variables[i] == 2) {
            delta_cost = obj_function.exchange_cost(c, i, sol);
            if (delta_cost < min_delta_cost) {
              min_delta_cost = delta_cost;
              cand_in = c;
              cand_out = i;
            }
          }
        }
      }

      if (min_delta_cost < 0) {
        if (cand_out != -1) {
          obj_function.remove(cand_out, &sol);
          if (obj_function.insertion_cost(cand_out, sol) < 0)
            cl.push_back(cand_out);
        } else {
          obj_function.insert(cand_in, &sol);
          cl.erase(remove(cl.begin(), cl.end(), cand_in), cl.end());
        }
      }
    } while (min_delta_cost < 0);
  }

  Solution solve() {
    auto start = chrono::high_resolution_clock::now();
    Solution best_sol = Solution(obj_function.size); // solução vazia
    for (int i = 0; i < interations; i++) {
      sol = Solution(obj_function.size);
      constructive_heuristic();
      local_search();
      if (best_sol.cost > sol.cost) {
        best_sol = Solution(sol);
        // TODO break in optimal
        cout << "Iter. " << i << "\t Best ";
        sol.print();
      }
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time:\t" << duration.count() << endl;
    return best_sol;
  }
};

int main(int argc, char *argv[]) {
  cout << argv[3] << endl;
  Grasp grasp = Grasp(stod(argv[1]), stoi(argv[2]), argv[3]);
  grasp.best = stoi(argv[4]);
  Solution sol = grasp.solve();
}
