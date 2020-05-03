int *rotate_piece(int *piece, int orientation) {
    int *rotated_piece = new int[4];
    if (orientation == 0) {
        rotated_piece[0] = piece[0];
        rotated_piece[1] = piece[1];
        rotated_piece[2] = piece[2];
        rotated_piece[3] = piece[3];
    } else if (orientation == 1) {
        rotated_piece[0] = piece[3];
        rotated_piece[1] = piece[0];
        rotated_piece[2] = piece[1];
        rotated_piece[3] = piece[2];
    } else if (orientation == 2) {
        rotated_piece[0] = piece[2];
        rotated_piece[1] = piece[3];
        rotated_piece[2] = piece[0];
        rotated_piece[3] = piece[1];
    } else if (orientation == 3) {
        rotated_piece[0] = piece[1];
        rotated_piece[1] = piece[2];
        rotated_piece[2] = piece[3];
        rotated_piece[3] = piece[0];
    }
    return rotated_piece; 
}

int *get_next_position(int cols, int *prev_position) {
    int *next_position = new int[2];
    next_position[0] = ((prev_position[0] * cols) + prev_position[1] + 1) / cols;
    next_position[1] = ((prev_position[0] * cols) + prev_position[1] + 1) % cols;
    return next_position;
}
