class Board {
public:
   Board(void) {
      for(size_t i=0; i<m_size; i++) {
         for(size_t j=0; j<m_size; j++) {
            m_grid[i][j] = false;
         }
      }
   }

   void set(size_t x, size_t y, bool val) {
      if(!valid(x, y)) return;
      m_grid[x][y] = val;
   }

   bool get(size_t x, size_t y) const {
      if(!valid(x, y)) return false;
      return m_grid[x][y];
   }
   
   size_t toRange(int val) const {
     val %= m_size;
     if(val < 0) val += m_size;
     return val;
   }
   
   int getModulus(int x, int y) const {
     return get(toRange(x), toRange(y));
   }

   size_t size() const {
      return m_size;
   }

private:
   bool valid(size_t x, size_t y) const {
      return (x < m_size && y < m_size);
   }

   static const size_t m_size = 8;
   bool m_grid[m_size][m_size];
};

bool operator==(const Board & lhs, const Board & rhs) {
  for(size_t i=0; i<lhs.size(); i++) {
    for(size_t j=0; j<rhs.size(); j++) {
      if(lhs.get(i, j) != rhs.get(i, j)) {
        return false;
      }
    }
  }
  return true;
}

bool check(size_t x, size_t y, const Board & board) {
   // Check the rows and colums
   for(size_t i=0; i<board.size(); i++) {
      if(board.get(x, i)) return false;
      if(board.get(i, y)) return false;
   }
   // Check the diagonals
   for(size_t i=1; i<board.size(); i++) {
      if(board.get(x+i, y+i)) return false;
      if(board.get(x+i, y-i)) return false;
      if(board.get(x-i, y+i)) return false;
      if(board.get(x-i, y-i)) return false;
   }
   return true;
}

bool solve(Board & board, size_t row = 0) {
   if(row >= board.size()) return true;
   for(size_t col=0; col<board.size(); col++) {
      if(check(col, row, board)) {
         board.set(col, row, true);
         if(solve(board, row+1)) {
            return true;
         }
         board.set(col, row, false);
      }
   }
   return false;
}
