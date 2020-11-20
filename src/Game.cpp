#include "Game.h"

#include <Windows.h>

Game::Game()
: m_columns(10)
, m_rows(15)
, m_mines(15)
, m_tilesToCheck(145)
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
}

void Game::InitMines(int x, int y)
{
    m_tilesToCheck = m_columns * m_rows;
    int mines = (m_columns * m_rows) * m_mines / 100;
    
    if(mines == 0)
    {
        mines++;
    }
    
    while(mines > 0)
    {
        int rx = rand() % m_columns;
        int ry = rand() % m_rows;
        
        if(m_tiles[ry * m_columns + rx] == 0 && rx != x && ry != y)
        {
            m_tiles[ry * m_columns + rx] = MINE;
            mines--;
            m_tilesToCheck--;
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
    
    int mines = 0;
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            if(x + i >= 0 && x + i < m_columns && y + j >= 0 && y + j < m_rows)
            {
                if(m_tiles[(y + j) * m_columns + (x + i)] == MINE)
                {
                    mines++;
                }
            }
        }
    }
    if(mines == 0)
    {
        m_tiles[y * m_columns + x] = CLEAR;
        return CLEAR;
    }
    if(mines > 0 && mines < 10)
    {
        m_tiles[y * m_columns + x] = mines;
        return (TILE_STATE)mines;
    }
    else
    {
        MessageBox(NULL, L"Too many mines on surrounding tiles, memory corruption?", L"Error", MB_OK | MB_ICONERROR);
    }
    
    return UNCHECKED;
}