/* date = November 20th 2020 3:04 pm */

#ifndef _GAME_H
#define _GAME_H

class Game
{
    private:
    int m_columns;
    int m_rows;
    int m_mines;
    
    public:
    Game();
    
    int get_columns() { return m_columns; }
    void set_columns(int columns) { m_columns = columns; }
    int get_rows() { return m_rows; }
    void set_rows(int rows) { m_rows = rows; }
    int get_mines() { return m_mines; }
    void set_mines(int mines) { m_mines = mines; }
};

#endif //_GAME_H
