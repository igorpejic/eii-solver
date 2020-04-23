class State(object):

    def __init__(self, grid, pieces, parent=None, solution_pieces_order=None):
        self.grid = np.copy(grid)
        self.pieces = pieces[:]
        self.parent = parent
        self.uuid = _uuid.uuid()
        self.children = []
        self.score = None
        self.tile_placed = None
        if parent and parent.solution_pieces_order:
            self.solution_pieces_order = parent.solution_pieces_order
        else:
            self.solution_pieces_order = []

    def uuid_str(self):
      return f'{self.uuid}'


    def copy(self):
        return State(self.grid, self.pieces, parent=self.parent)

    def child_with_biggest_score(self):
        return sorted(self.children, key=sort_key, reverse=True)[0]

    def render_to_json(self):
        return render_to_json(self)

    def render_children(self, only_ids=False):
        ret, all_nodes = render_to_dict(self, only_ids=only_ids)
        if only_ids:
            self_str = self.uuid_str()
        else:
            self_str = str(self)
        all_nodes[self.uuid_str()] = self
        return {self_str: ret}, all_nodes 

    def to_json(self):
        score = self.score or 0
        grid = list(self.grid)
        pieces = self.pieces
        tile_placed = self.tile_placed
        grid = self.grid.tolist()
        ret = {
            'pieces': pieces,
            'grid': grid,
            'tile_placed': tile_placed,
            'score': int(score),
            'name': self.uuid_str(),
            'children': [],
        }
        return ret


    def __str__(self):
        return self.__repr__()

    def __repr__(self, short=False):
        if short:
            return f'{self.pieces}'

        output_list = [list(x) for x in self.pieces]
        if len(output_list) > 6:
            output_list = str(output_list[:6]) +  '(...)'

        output_grid = ''
        if len(self.pieces) <= 4:
            output_grid = self.grid
        return f'({self.uuid}) Remaining pieces: {len(self.pieces) / ORIENTATIONS}, Tile placed: {self.tile_placed}. pieces: {output_list}. Sim. depth:({self.score}) {output_grid}'
