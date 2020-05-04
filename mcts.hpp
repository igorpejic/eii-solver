#include <vector>
#include <array>
#include <tuple>

struct state_t {       
    std::vector<std::array<int, 4>> m_pieces;        
    int *m_grid;        
    std::vector<state_t> m_children;
};

class CustomMCTS {       
  public:             
    int n_pieces_placed = 0;        
    int *m_initial_grid;
    int m_rows;
    int m_cols;
    int m_pieces_placed = 0;
    state_t m_state;
    std::vector<std::array<int, 4>> m_initial_pieces;

    CustomMCTS(std::vector<std::array<int, 4>> pieces, int rows, int cols);
    std::tuple<state_t, int, bool> predict(int);
    std::tuple<bool, int, std::vector<std::array<int, 6>>> perform_simulations(state_t state, int* next_position, int N);
    std::tuple<int, std::vector<std::array<int, 6>>> CustomMCTS::perform_simulation(state_t state, int *next_position);
};
