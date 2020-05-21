#include "solver.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <map>
#include <tuple>
#include <cmath>
#include "mcts.hpp"
#define MAX_DEPTH 100
#define AVG_DEPTH 101
#define SIMULATION_REPEATS 5


using namespace std;

void print_solution(std::vector<std::pair<std::array<int, 4>, std::array<int, 2>>> solution_pieces_order, int rows);

void print_solution(std::vector<std::pair<std::array<int, 4>, std::array<int, 2>>> solution_pieces_order, int rows) {
    for (int i = 0; i < rows * rows; i++) {
        if (i % rows == 0) {
            std::cout << std::endl;
        }
        std::cout << solution_pieces_order[i].first[0] << " "
            << solution_pieces_order[i].first[1] << " "
            << solution_pieces_order[i].first[2] << " "
            << solution_pieces_order[i].first[3] << "   ";
    }
    std::cout << std::endl;
}

int main (int argc, const char* argv[]) {
    std::vector<std::array<int, 4>> pieces = initialize_pieces(argv[1]);
    int rows = sqrt(pieces.size());
    std::array<int, 6> nRolloutsChoices = {100, 200, 500, 1000, 2000, 10000};
    std::map<int, std::string> strategies;
    strategies[MAX_DEPTH] = "maxDepth";
    strategies[AVG_DEPTH] = "avgDepth";
    //for (auto nRollouts : nRolloutsChoices) {
        //for ( const auto &[strategy, value]: strategies ) {
                //for (int i = 0; i < SIMULATION_REPEATS; i++) {
                int strategy = AVG_DEPTH;
                int nRollouts = 1000;
                CustomMCTS mcts(initialize_pieces(argv[1]), rows, rows, strategy);
                tuple<state_t, int, bool> result;
                result = mcts.predict(nRollouts);
                cout << "In total placed tiles:" << mcts.m_pieces_placed << std::endl;
                cout << "Pieces placed:" << mcts.m_solution_pieces_order.size() << std::endl;
                if (mcts.m_solution_pieces_order.size() == rows * rows) {
                    print_solution(mcts.m_solution_pieces_order, rows);
                }

                ///std::ofstream outfile;

                //outfile.open("results2.csv", std::ofstream::app);
                std::time_t timestamp = std::time(nullptr);
                //outfile << std::put_time(std::localtime(&timestamp), "%y-%m-%d %OH:%OM:%OS") << ";";
                //outfile << argv[1] << ";";
                //outfile << rows << ";";
                //outfile << strategies[strategy] << ";";
                //outfile << nRollouts << ";";
                //outfile << mcts.m_pieces_placed << ";";
                //outfile << std::get<2>(result) << ";";
                //outfile << std::get<1>(result) << ";";
                //outfile << std::endl;
                //}
        //}
    //}
    return 0;
}
