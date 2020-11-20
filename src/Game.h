/* date = November 20th 2020 3:04 pm */

#ifndef _GAME_H
#define _GAME_H

typedef enum
{
    UNCHECKED = -1,
    
    CLEAR = 0,
    
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    
    MINE = 10,
    EXPLODE = 11,
    FLAGGED_CORRECT = 12,
    FLAGGED_INCORRECT = 13
} TILE_STATE;

class Game
{
    private:
    int m_columns;
    int m_rows;
    int m_mines;
    int m_tilesToCheck;
    int* m_tiles;
    
    void CleanUp();
    
    public:
    Game();
    ~Game();
    void InitGame();
    void InitMines(int x, int y);
    
    
    int get_columns() { return m_columns; }
    void set_columns(int columns) { m_columns = columns; }
    int get_rows() { return m_rows; }
    void set_rows(int rows) { m_rows = rows; }
    int get_mines() { return m_mines; }
    void set_mines(int mines) { m_mines = mines; }
    
    TILE_STATE GetTileState(int x, int y);
};

#endif //_GAME_H
