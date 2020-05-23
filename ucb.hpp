#define EMPTY 8 


extern unsigned long long int zobrist_table[PUZZLE_SIZE][PUZZLE_SIZE][PUZZLE_SIZE * PUZZLE_SIZE * 4]; 

int play_game(placed_pieces _placed_pieces, Piece** rotated_pieces, neighbours_map_t &neighbours_map, board board, Position position, int * const tiles_placed, int *max_pieces_placed, std::default_random_engine rng, bool *solution_found);

class Node {
    typedef std::vector<Node> node_children_t;
    public:
        board m_board;
        placed_pieces m_placed_pieces;
        Position m_next_position;

        bool m_is_terminal;
        bool m_is_solution;

        Node(board &board, placed_pieces _placed_pieces, Position next_position, bool is_terminal, bool is_solution);
        Node find_random_child();
        Node find_random_child(Piece **rotated_pieces, neighbours_map_t &neighbours_map, int *tiles_placed);
        node_children_t find_children(Piece **rotated_pieces, neighbours_map_t &neighbours_map, int *tiles_placed);
        Position next_position;

        Node make_move(PiecePlacement piece_placement, Piece **rotated_pieces, neighbours_map_t &neighbours_map);
        bool operator==(const Node &n1) const;
};

namespace std {

  template <>
  struct hash<Node>
  {
    std::size_t operator()(const Node& n) const
    {
      // https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-5-zobrist-hashing/
      unsigned long long int h = 0; 
      for (int i = 0; i<PUZZLE_SIZE; i++) { 
        for (int j = 0; j<PUZZLE_SIZE; j++) { 
            if (n.m_board[i * PUZZLE_SIZE + j].orientation != EMPTY) 
            { 
                int piece = n.m_board[i * PUZZLE_SIZE + j].index * 4 + n.m_board[i * PUZZLE_SIZE + j].orientation; 
                h ^= zobrist_table[i][j][piece]; 
            } 
        } 
      }
      return h; 
    } 
  };
}

typedef std::vector<Node> path_t;

class MCTS {
    public:
        MCTS(Piece** rotated_pieces, neighbours_map_t &neighbours_map);
        Piece** m_rotated_pieces;
        neighbours_map_t m_neighbours_map;
        std::unordered_map<Node, std::vector<Node>> m_children;
        std::unordered_map<Node, int> m_Q;
        std::unordered_map<Node, int> m_N;

        Node choose(Node node);

        Node _uct_select(Node &node);
        path_t _select(Node &node);

        Node *m_solution_node;

        void _expand(Node &node);

        bool do_rollout(Node &node);

        double _simulate(Node &node);

        void _backpropagate(path_t &path, double reward);

        int m_tiles_placed;
};

