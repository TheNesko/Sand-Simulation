#pragma once
#include "raylib.h"
#include "utils.h"
#include "particle.h"
#include <vector>
#include <cmath>

Vector2 getGridPosition(Vector2 position, const int& cellSize);

Vector2 getWorldPosition(Vector2 gridPosition, const int& cellSize);

void generateBorders(std::vector<std::vector<particle>> &grid, const int width, const int height);

IntVector2 getClosestFreeCell(int substanceId,int x, int y, int velocity, IntVector2 direction, std::vector<std::vector<particle>> &grid);

void updateSand(int x, int y, std::vector<std::vector<particle>> &grid, const int gridWidth, const int gridHeight);

void updateWater(int x, int y, std::vector<std::vector<particle>> &grid, const int gridWidth, const int gridHeight);

void updateFire(int x, int y, std::vector<std::vector<particle>> &grid, const int gridWidth, const int gridHeight);
