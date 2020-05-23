#include <vector>
#include <cstring>
#include <tuple>
#include <array>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <random>
#include <cmath>
#include <climits>
#include "mcts.hpp"
#include "solver.hpp"
#include "ucb.hpp"

#include  <random>
#include  <iterator>

#define SOLUTION_FOUND -1

using namespace std;

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

unsigned long long int zobrist_table[PUZZLE_SIZE][PUZZLE_SIZE][PUZZLE_SIZE * PUZZLE_SIZE * 4]; 
std::mt19937 mt(01234567);  // reproducibility
//std::mt19937 mt(time(NULL)); 
  
unsigned long long int random_int() 
{ 
    //uniform_int_distribution<unsigned long long int> dist(0, UINT64_MAX); 
    std::uniform_int_distribution<unsigned long long int> dist(0, ULLONG_MAX); 
    return dist(mt); 
} 

void initialize_zobrist_table() {
    for (int i = 0; i < PUZZLE_SIZE; i++) {
        for (int j = 0; j < PUZZLE_SIZE; j++) {
            for (int k = 0; k < PUZZLE_SIZE * PUZZLE_SIZE * 4; k++)  {
                //std::cout << random_int() << std::endl;
                zobrist_table[i][j][k] = random_int(); 
            }
        }
    }
}

int play_game(placed_pieces _placed_pieces, Piece** rotated_pieces, neighbours_map_t &neighbours_map, board board, Position position, int * const tiles_placed, int *max_pieces_placed, std::default_random_engine rng, bool *solution_found) { 

    const int n_rollouts = 10;

    initialize_zobrist_table();

    int n_restarts = 0;
    //TODO: start from 0, 1
    Position start_position;
    start_position.i = 0;
    start_position.j = 0;
    Node node(board, _placed_pieces, start_position, false, false);

    MCTS mcts(rotated_pieces, neighbours_map);

    while(true) {
        if (node.m_is_terminal) {
            // terminal node
            //std::cout << "terminal node" << std::endl;
            node = Node(board, _placed_pieces, start_position, false, false);
            n_restarts += 1;
        }
        node = mcts.choose(node); 
        for (int i = 0; i < n_rollouts; i++) {
            bool solution_found = mcts.do_rollout(node);
            if(solution_found) {
                //std::cout << mcts.m_tiles_placed << std::endl;
                return mcts.m_tiles_placed;
            }
        }
        cout << "q_size:" << mcts.m_Q.size() << endl;
        //int i = 0;
        //for (auto kv : mcts.m_Q) {
        //    cout << i << ":" << +kv.first.m_next_position.i << " " << +kv.first.m_next_position.j << endl;
        //    i++;
        //}
        //std::cout << node.m_placed_pieces.count() << std::endl;
        //std::cout << mcts.m_tiles_placed << " " << mcts.m_tiles_placed % 100000 << std::endl;
    }
}

MCTS::MCTS(Piece** rotated_pieces, neighbours_map_t &neighbours_map) {
    m_rotated_pieces = rotated_pieces;
    m_neighbours_map = neighbours_map;
    std::unordered_map<Node, std::vector<Node>> m_children;
    std::unordered_map<Node, int> m_Q;
    std::unordered_map<Node, int> m_N;
    m_tiles_placed = 0;
}


Node MCTS::choose(Node node) {
    if (node.m_is_terminal) {
        std::cout << "This should never happen!!!" << std::endl;
    }
    if (m_children.find(node) == m_children.end()) {
        return node.find_random_child(m_rotated_pieces, m_neighbours_map, &m_tiles_placed);
    } else {
        int best_score = 0;
        Node best_node = m_children[node][0];
        for (int i = 0; i < m_children[node].size(); i++) {
            double avg_reward = (double) m_Q[m_children[node][i]] / m_N[m_children[node][i]];
            if(avg_reward > best_score) { 
                best_score = avg_reward;
                best_node = m_children[node][i];
            }
        }
        return best_node;
    }
}


bool MCTS::do_rollout(Node &node) {
    path_t path = this->_select(node);
    Node leaf = path.back();
    if (leaf.m_is_solution) {
        std::cout << "Solution found." << std::endl;
        print_final_solution(leaf.m_board, this->m_rotated_pieces);
        std::cout << "Tiles placed" << std::endl;
        return true;
    }
    this->_expand(leaf);
    double reward = this->_simulate(node);
    if (reward < 0) {
        print_final_solution(this->m_solution_node->m_board, this->m_rotated_pieces);
        delete(this->m_solution_node);
        return true;
    }
    //cout << "reward" << reward << endl;
    this->_backpropagate(path, reward);
    return false;
}


void MCTS::_backpropagate(path_t &path, double reward) { 
    //Send the reward back up to the ancestors of the leaf
    for (int i = 0; i < path.size(); i++) {
        //cout << "PATH:" << path[i].m_placed_pieces << "--"  << m_Q.size() << std::endl;
        bool in_q = m_Q.find(path[i]) == m_Q.end();
        //cout << "PATH2:" << in_q  << std::endl;
        m_N[path[i]] += 1;
        m_Q[path[i]] += reward;
    }
    //std::cout << reward << std::endl;
}

path_t MCTS::_select(Node &node) {
    // Find an unexplored descendent of `node`
    Node current_node = node;
    path_t result_path;

    while(true) {
        result_path.push_back(current_node);
        if (m_children.find(current_node) == m_children.end() || m_children[current_node].size() == 0) {
            // either unexplored node; or terminal node
            return result_path;
        }

        //TODO (performance): replace m_children with set
        for(int i = 0; i < m_children[current_node].size(); i++) {
            if(m_children.find(m_children[current_node][i]) == m_children.end()) {
                result_path.push_back(m_children[current_node][i]);
                return result_path;
            }
        }
        //descend a layer deeper
        current_node = this->_uct_select(current_node);
    }
}

Node MCTS::_uct_select(Node &node) {
    //Select a child of node, balancing exploration & exploitation"
    //
    double log_N_vertex = log(m_N[node]);

    double exploration_weight = 1;

    double best_score = 0;
    Node best_node = m_children[node][0];
    for (int i = 0; i < m_children[node].size(); i++) {
        double ucb = m_Q[node] / m_N[node] +  sqrt(log_N_vertex / m_N[node]);
        if(ucb > best_score) { 
            best_score = ucb;
            best_node = m_children[node][i];
        }
    }
    return best_node;
}

void MCTS::_expand(Node &node) {
    //Update the `children` dict with the children of `node`
    if(m_children.find(node) != m_children.end()) {
        return;
    }
    //cout << "expand" << std::endl;
    m_children[node] = node.find_children(m_rotated_pieces, m_neighbours_map, &m_tiles_placed);
}

double MCTS::_simulate(Node &node) {
    //Returns the reward for a random simulation (to completion) of `node`"
    Node current_node = node;
    while(true) {
        //cout << "simulation:" << current_node.m_placed_pieces << std::endl;
        if (current_node.m_is_solution) {
            std::cout << "solution found in simulation" << std::endl;
            this->m_solution_node = new Node(current_node);
            return SOLUTION_FOUND;
        } else if (current_node.m_is_terminal) {
            double reward = (double)current_node.m_placed_pieces.count() / (PUZZLE_SIZE * PUZZLE_SIZE);
            return reward;
        }
        current_node = current_node.find_random_child(m_rotated_pieces, m_neighbours_map, &m_tiles_placed);
    }
}



Node::Node(board &board, placed_pieces _placed_pieces, Position next_position, bool is_terminal, bool is_solution) {
    m_board = board;
    m_placed_pieces = _placed_pieces;
    m_is_terminal = is_terminal;
    m_is_solution = is_solution;
    m_next_position = next_position;
}

bool Node::operator==(const Node &n2) const {
    if (m_placed_pieces != n2.m_placed_pieces) {
        return false;
    } else if (!(m_next_position == n2.m_next_position)) {
        return false;
    } else if (m_board != n2.m_board) {
        return false;
    }
    return true;
}


Node Node::find_random_child(Piece **rotated_pieces, neighbours_map_t &neighbours_map, int *tiles_placed) {

    std::vector<PiecePlacement> next_moves = get_valid_next_moves_b(
            m_board, m_placed_pieces, neighbours_map, m_next_position, rotated_pieces);

    PiecePlacement random_move = *select_randomly(next_moves.begin(), next_moves.end());
    Node new_node = make_move(random_move, rotated_pieces, neighbours_map);
    (*tiles_placed)++;
    return new_node;
}

Node::node_children_t Node::find_children(Piece **rotated_pieces, neighbours_map_t &neighbours_map, int *tiles_placed) {
    std::vector<PiecePlacement> next_moves = get_valid_next_moves_b(
            m_board, m_placed_pieces, neighbours_map, m_next_position, rotated_pieces);

    Node::node_children_t children;
    if (!next_moves.size()) {
        return children;
    }
    for(int i = 0; i < next_moves.size(); i++) {
        Node new_node = make_move(next_moves[i], rotated_pieces, neighbours_map);
        (*tiles_placed)++;
        children.push_back(new_node);
    }
    return children;
}

Node Node::make_move(PiecePlacement piece_placement, Piece **rotated_pieces, neighbours_map_t &neighbours_map) {
    board new_board = m_board;
    new_board[m_next_position.i * PUZZLE_SIZE + m_next_position.j] = piece_placement;
    placed_pieces new_placed_pieces = m_placed_pieces;
    new_placed_pieces[piece_placement.index] = true;
    Position new_next_position = get_next_position_b(PUZZLE_SIZE, m_next_position);

    std::vector<PiecePlacement> next_moves = get_valid_next_moves_b(
            new_board, new_placed_pieces, neighbours_map, new_next_position, rotated_pieces);

    bool is_terminal = !(next_moves.size()) ? true : false;
    Node new_node(new_board, new_placed_pieces, new_next_position, is_terminal, new_placed_pieces.all());
    return new_node;
}
