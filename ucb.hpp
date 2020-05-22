void play_game(pieces &pieces, placed_pieces _placed_pieces, Piece** rotated_pieces, neighbours_map_t &neighbours_map, board board, Position position, int * const tiles_placed, int *max_pieces_placed, std::default_random_engine rng, bool *solution_found);

class Node {
    public:
        board m_board;
        placed_pieces m_placed_pieces;

        bool m_is_terminal;

        Node(board &board, placed_pieces _placed_pieces);
};

namespace std {

  template <>
  struct hash<Node>
  {
    std::size_t operator()(const Node& n) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      //TODO: implement Zobrist hashing
      return 1;
    }
  };

}

class MCTS {
    public:
        int m_dummy;

        MCTS(int dummy);
        std::unordered_map<Node, std::vector<Node>> m_children;

        Node choose(Node node);
};
