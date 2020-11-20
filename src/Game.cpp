#include "Game.h"

#include <Windows.h>

Game::Game()
: m_columns(10)
, m_rows(15)
, m_mines(20)
, m_tilesToCheck(130)
, m_tiles(nullptr)
{
}

Game::~Game()
{
    CleanUp();
}

void Game::CleanUp()
{
    if(m_tiles)
    {
        GlobalFree(m_tiles);
    }
}

void Game::InitGame()
{
    CleanUp();
    
    m_tiles = (int*)GlobalAlloc(GPTR, sizeof(int) * m_rows * m_columns);
    
    InitMines(0, 0);
    m_tilesToCheck = m_columns * m_rows - m_mines;
}

void Game::InitMines(int x, int y)
{
    int mines = m_mines;
    
    while(mines > 0)
    {
        int rx = rand() % m_columns;
        int ry = rand() % m_rows;
        
        if(m_tiles[ry * m_columns + rx] == 0 && rx != x && ry != y)
        {
            m_tiles[ry * m_columns + rx] = MINE;
            mines--;
        }
    }
}

TILE_STATE Game::GetTileState(int x, int y)
{
    if(!m_tiles)
    {
        MessageBox(NULL, L"Board not initalized!", L"Error", MB_OK | MB_ICONERROR);
        return UNCHECKED;
    }
    
    TILE_STATE result = (TILE_STATE)m_tiles[y * m_columns + x];
    return result;
}

TILE_STATE Game::CheckTileState(int x, int y)
{
    if(!m_tiles)
    {
        MessageBox(NULL, L"Board not initalized!", L"Error", MB_OK | MB_ICONERROR);
        return UNCHECKED;
    }
    
    m_tilesToCheck--;
    
    TILE_STATE result = (TILE_STATE)m_tiles[y * m_columns + x];
    if(result == MINE)
    {
        m_tiles[y * m_columns + x] = EXPLODE;
        return EXPLODE;
    }
    
    return UNCHECKED;
}