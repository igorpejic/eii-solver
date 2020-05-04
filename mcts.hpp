#include <vector>
#include <utility>      // std::pair, std::make_pair
#include <array>
#include <tuple>

struct state_t {       
    std::vector<std::array<int, 4>> m_pieces;        
    std::array<int, 4> m_piece_placed;        
    int *m_grid;        
    std::vector<state_t> m_children;
    float m_score;
    std::array<int, 2> m_next_position;
};

class CustomMCTS {       
  public:             
    int n_pieces_placed = 0;        
    int *m_initial_grid;
    int m_rows;
    int m_cols;
    int m_pieces_placed = 0;
    int m_strategy;
    state_t m_state;
    std::vector<std::array<int, 4>> m_initial_pieces;
    std::vector<std::pair<std::array<int, 4>, std::array<int, 2>>> m_solution_pieces_order;

    CustomMCTS(std::vector<std::array<int, 4>> pieces, int rows, int cols, int strategy);
    std::tuple<state_t, int, bool> predict(int);
    std::tuple<bool, float, std::vector<std::pair<std::array<int, 4>, std::array<int, 2> > > > perform_simulations(state_t state, std::array<int, 2> next_position, int N);
    std::tuple<int, std::vector<std::pair<std::array<int, 4>, std::array<int, 2> > >> perform_simulation(state_t state, std::array<int, 2> next_position);
};

int get_max_index(std::vector<state_t> states);
