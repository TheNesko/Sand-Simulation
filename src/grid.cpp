#include "grid.h"

Vector2 getGridPosition(Vector2 position, const int& cellSize){
    Vector2 gridPosition = { (float)floor(position.x/cellSize), (float)floor(position.y/cellSize) };
    return gridPosition;
}

Vector2 getWorldPosition(Vector2 gridPosition, const int& cellSize){
    Vector2 worldPosition = { gridPosition.x*cellSize, gridPosition.y*cellSize };
    return worldPosition;
}

void generateBorders(std::vector<std::vector<particle>> &grid, const int width, const int height){
    particle border;
    border.id = -1;
    for (int x=0; x < width; x++){
        grid[x][0] = border;
        grid[x][height-1] = border;
    }
    for (int y=0; y < height; y++){
        grid[0][y] = border;
        grid[width-1][y] = border;
    }
}

IntVector2 getClosestFreeCell(int substanceId,int x, int y, int velocity, IntVector2 direction, std::vector<std::vector<particle>> &grid){
    if (velocity > 0){
        if (substanceId == 1 or substanceId == 4){
            if (grid[x+direction.x][y+direction.y].id == 0 or grid[x+direction.x][y+direction.y].id == 2) {
                return getClosestFreeCell(substanceId, x+direction.x, y+direction.y, velocity-1, direction, grid);
            }
        }
        else if (grid[x+direction.x][y+direction.y].id == 0) {
            return getClosestFreeCell(substanceId, x+direction.x, y+direction.y, velocity-1, direction, grid);
        }
    }
    return {x, y};
}

void updateSand(int x, int y, std::vector<std::vector<particle>> &grid, const int gridWidth, const int gridHeight){
    if (x > 0 and x < gridWidth-1 and y > 0 and y < gridHeight-1){
        IntVector2 dir = {0, 0};
        if (grid[x][y+1].id == 0 or grid[x][y+1].id == 2){
            dir = {0, 1};
        }
        else {
            int rng = rand() %2;
            if (rng == 0) {
                if (grid[x-1][y+1].id == 0 or grid[x-1][y+1].id == 2){
                    dir = {-1, 1};
                }
                else if (grid[x+1][y+1].id == 0 or grid[x+1][y+1].id == 2){
                    dir = {1, 1};
                }
            }
            else if (rng == 1) {
                if (grid[x+1][y+1].id == 0 or grid[x+1][y+1].id == 2){
                    dir = {1, 1};
                }
                else if (grid[x-1][y+1].id == 0 or grid[x-1][y+1].id == 2){
                    dir = {-1, 1};
                }
            }
        }

        if (dir.x != 0 or dir.y != 0){
            particle temp = grid[x][y];
            IntVector2 newPos = getClosestFreeCell(temp.id, x, y, temp.velocity, dir, grid);
            grid[x][y] = grid[newPos.x][newPos.y];
            grid[newPos.x][newPos.y] = temp;
            grid[newPos.x][newPos.y].hasBeenUpdated = true;
            grid[x][y].hasBeenUpdated = true;
        }
    }
}

void updateWater(int x, int y, std::vector<std::vector<particle>> &grid, const int gridWidth, const int gridHeight){
    if (x > 0 and x < gridWidth-1 and y > 0 and y < gridHeight-1){
        IntVector2 dir = {0, 0};
        if (grid[x][y+1].id == 0){
            dir = {0, 1};
        }
        else {
            int rng = rand() %2;
            if (rng == 0) {
                if (grid[x-1][y+1].id == 0) {
                    dir = {-1, 1};
                }
                else if (grid[x+1][y+1].id == 0) {
                    dir = {1, 1};
                }
                else if (grid[x+1][y].id == 0) {
                    dir = {1, 0};
                }
                else if (grid[x-1][y].id == 0) {
                    dir = {-1, 0};
                }
            }
            else if (rng == 1) {
                if (grid[x+1][y+1].id == 0) {
                    dir = {1, 1};
                }
                else if (grid[x-1][y+1].id == 0){
                    dir = {-1, 1};
                }
                else if (grid[x-1][y].id == 0) {
                    dir = {-1, 0};
                }
                else if (grid[x+1][y].id == 0) {
                    dir = {1, 0};
                }
            }
        }

        if (dir.x != 0 or dir.y != 0) {
            particle temp = grid[x][y];
            IntVector2 newPos = getClosestFreeCell(temp.id, x, y, temp.velocity, dir, grid);
            grid[x][y] = grid[newPos.x][newPos.y];
            grid[newPos.x][newPos.y] = temp;
            grid[newPos.x][newPos.y].hasBeenUpdated = true;
        }
    }
}

void updateFire(int x, int y, std::vector<std::vector<particle>> &grid, const int gridWidth, const int gridHeight){
    if (x > 0 and x < gridWidth-1 and y > 0 and y < gridHeight-1) {

        if (grid[x][y-1].id == 5) {  // TODO: ADD FLAMABILITY TO PARTICLES AND DEPENDING ON IT ROLL A NUMBER AND CHECK IF PARTICLE SHOULD BURN OR NOT
            grid[x][y-1] = grid[x][y];
            grid[x][y-1].lifeTime = 20;
            grid[x][y-1].hasBeenUpdated = true;
        }
        if (grid[x][y+1].id == 5) {
            grid[x][y+1] = grid[x][y];
            grid[x][y+1].lifeTime = 20;
            grid[x][y+1].hasBeenUpdated = true;
        }
        if (grid[x+1][y].id == 5) {
            grid[x+1][y] = grid[x][y];
            grid[x+1][y].lifeTime = 20;
            grid[x+1][y].hasBeenUpdated = true;
        }
        if (grid[x-1][y].id == 5) {
            grid[x-1][y] = grid[x][y];
            grid[x-1][y].lifeTime = 20;
            grid[x-1][y].hasBeenUpdated = true;
        }

        IntVector2 dir = {0, 0};
        if (grid[x][y-1].id == 0){
            dir = {0, -1};
        }
        else {
            int rng = rand() %2;
            if (rng == 0) {
                if (grid[x-1][y-1].id == 0) {
                    dir = {-1, -1};
                }
                else if (grid[x+1][y].id == 0) {
                    dir = {1, 0};
                }
                else if (grid[x+1][y-1].id == 0) {
                    dir = {1, -1};
                }
                else if (grid[x-1][y].id == 0) {
                    dir = {-1, 0};
                }
            }
            else if (rng == 1) {
                if (grid[x+1][y-1].id == 0) {
                    dir = {1, -1};
                }
                else if (grid[x-1][y].id == 0) {
                    dir = {-1, 0};
                }
                else if (grid[x-1][y-1].id == 0){
                    dir = {-1, -1};
                }
                else if (grid[x+1][y].id == 0) {
                    dir = {1, 0};
                }
            }
        }

        grid[x][y].lifeTime--;
        if (grid[x][y].lifeTime <= 0) { 
            grid[x][y].id = 0;
            return;
        }
        if (dir.x != 0 or dir.y != 0){
            particle temp = grid[x][y];
            IntVector2 newPos = getClosestFreeCell(temp.id, x, y, temp.velocity, dir, grid);
            grid[x][y] = grid[newPos.x][newPos.y];
            grid[newPos.x][newPos.y] = temp;
            grid[newPos.x][newPos.y].hasBeenUpdated = true;
            // grid[x][y].hasBeenUpdated = true;
        }
    }
}
