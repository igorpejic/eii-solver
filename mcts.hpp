#include <vector>
#include <array>

class CustomMCTS {       
  public:             
    int n_pieces_placed = 0;        
    std::vector<std::array<int, 4>> initial_pieces;
    CustomMCTS(std::vector<std::array<int, 4>> pieces);
};
