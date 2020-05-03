int *rotate_piece(int *piece, int orientation);
int *get_next_position(int cols, int *prev_position);
int ***initialize_grid(int rows, int cols);
int **initialize_pieces(const char *filename);
int **initialize_pieces(const char *filename);
bool is_move_legal(int ***grid, int *piece, int *position, int rows, int cols);
