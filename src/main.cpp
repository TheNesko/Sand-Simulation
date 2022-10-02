#include "raylib.h"
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>

const int targetFPS = 60;
const int screenWidth = 1280;
const int screenHeight = 960;
const int cellSize = 4;
const int gridWidth = screenWidth/cellSize;
const int gridHeight = screenHeight/cellSize;

struct IntVector2 {
    int x, y;
};

void clamp(int& value, int min, int max){
    if (value > max) value = max;
    if (value < min) value = min;
}

Vector2 normalize(Vector2 vector){
    Vector2 normalized;
    float length = sqrt(vector.x*vector.x + vector.y*vector.y);
    normalized.x = vector.x / length;
    normalized.y = vector.y / length;
    return normalized;
}

Vector2 getGridPosition(Vector2 position, const int& cellSize){
    Vector2 gridPosition = { (float)floor(position.x/cellSize), (float)floor(position.y/cellSize) };
    return gridPosition;
}

Vector2 getWorldPosition(Vector2 gridPosition, const int& cellSize){
    Vector2 worldPosition = { gridPosition.x*cellSize, gridPosition.y*cellSize };
    return worldPosition;
}

struct particle {
    int id;
    float lifeTime;
    int velocity;
    Color color;
    bool hasBeenUpdated;
};

void generateBorders(std::vector<std::vector<particle>> &grid){
    particle border;
    border.id = -1;
    for (int x=0; x < gridWidth; x++){
        grid[x][0] = border;
        grid[x][gridHeight-1] = border;
    }
    for (int y=0; y < gridHeight; y++){
        grid[0][y] = border;
        grid[gridWidth-1][y] = border;
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

void updateSand(int x, int y, std::vector<std::vector<particle>> &grid){
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

void updateWater(int x, int y, std::vector<std::vector<particle>> &grid){
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

void updateFire(int x, int y, std::vector<std::vector<particle>> &grid){
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


int main(void)
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Game");

    std::vector<std::vector <particle>> grid;
    grid.resize(gridWidth);
    for (int x=0; x < gridWidth; x++){
        grid[x].resize(gridHeight);
    }

    generateBorders(grid);
    SetTargetFPS(targetFPS);

    particle sand;
    sand.id = 1;
    sand.lifeTime = -1;
    sand.color = {255, 255, 0, 255};
    sand.velocity = 2;
    sand.hasBeenUpdated = false;
    particle water;
    water.id = 2;
    water.lifeTime = -1;
    water.color = {0, 0, 255, 255};
    water.velocity = 4;
    water.hasBeenUpdated = false;
    particle stone;
    stone.id = 3;
    stone.lifeTime = -1;
    stone.color = {50, 50, 50, 255};
    stone.velocity = 0;
    particle dirt;
    dirt.id = 4;
    dirt.lifeTime = -1;
    dirt.color = {87, 62, 40, 255};
    dirt.velocity = 1;
    dirt.hasBeenUpdated = false;
    particle wood;
    wood.id = 5;
    wood.lifeTime = -1;
    wood.color = {61, 37, 16, 255};
    wood.velocity = 0;
    particle fire;
    fire.id = 6;
    fire.lifeTime = 20; //life time (ammount of frames particle will life)
    fire.color = {138, 11, 4, 255};
    fire.velocity = 2;
    fire.hasBeenUpdated = false;


    particle selected = sand;

    int brushSize = 2;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        srand( time(NULL) );
        if (IsKeyPressed(KEY_R)) {
            for (int x=0; x < gridWidth; x++){
                for (int y=0; y < gridWidth; y++){
                    grid[x][y].id = 0;
                }
            }
        }

        if (IsKeyPressed(KEY_UP)) {
            brushSize+=1;
            clamp(brushSize, 1, 40);
        }
        if (IsKeyPressed(KEY_DOWN)) {
            brushSize-=1;
            clamp(brushSize, 1, 40);
        }

        if (GetMouseWheelMove() != 0) {
            brushSize+=(int)GetMouseWheelMove();
            clamp(brushSize, 1, 40);
        }

        if (IsKeyPressed(KEY_ONE)) selected = sand;
        if (IsKeyPressed(KEY_TWO)) selected = water;
        if (IsKeyPressed(KEY_THREE)) selected = stone;
        if (IsKeyPressed(KEY_FOUR)) selected = dirt;
        if (IsKeyPressed(KEY_FIVE)) selected = wood;
        if (IsKeyPressed(KEY_SIX)) selected = fire;
     
        Vector2 mouse = GetMousePosition();
        Vector2 gridMouse = getGridPosition(mouse, cellSize);
        int gridMouseX = gridMouse.x;
        int gridMouseY = gridMouse.y;
        Vector2 worldMouse = getWorldPosition(gridMouse, cellSize);

        if (IsMouseButtonDown(0)) {
            if (brushSize > 1) {
                for (int x=-brushSize/2; x < brushSize/2; x++) {
                    for (int y=-brushSize/2; y < brushSize/2; y++) {
                        if (gridMouseX+x >= 0 and gridMouseX+x <= gridWidth-1 and gridMouseY+y >= 0 and gridMouseY+y <= gridHeight-1){
                            if (grid[gridMouseX+x][gridMouseY+y].id == 0) {
                                grid[gridMouseX+x][gridMouseY+y] = selected;
                                // grid[gridMouseX+x][gridMouseY+y].id = selected.id;
                                // grid[gridMouseX+x][gridMouseY+y].color = selected.color;
                                // grid[gridMouseX+x][gridMouseY+y].velocity = selected.velocity;
                                // grid[gridMouseX+x][gridMouseY+y].hasBeenUpdated = selected.hasBeenUpdated;
                            }
                        }
                    }
                }
            }

            else {
                if (gridMouseX >= 0 and gridMouseX <= gridWidth-1 and gridMouseY >= 0 and gridMouseY <= gridHeight-1){
                    if (grid[gridMouseX][gridMouseY].id == 0) {
                        grid[gridMouseX][gridMouseY] = selected;
                        // grid[gridMouseX][gridMouseY].id = selected.id;
                        // grid[gridMouseX][gridMouseY].color = selected.color;
                        // grid[gridMouseX][gridMouseY].velocity = selected.velocity;
                    }
                }
            }
        }

        if (IsMouseButtonDown(1)) {
            if (brushSize > 1) {
                for (int x=-brushSize/2; x < brushSize/2; x++) {
                    for (int y=-brushSize/2; y < brushSize/2; y++) {
                        if (gridMouseX+x >= 0 and gridMouseX+x <= gridWidth-1 and gridMouseY+y >= 0 and gridMouseY+y <= gridHeight-1){
                            if (grid[gridMouseX+x][gridMouseY+y].id > 0) {
                                grid[gridMouseX+x][gridMouseY+y] = particle();
                                grid[gridMouseX+x][gridMouseY+y].id = 0;
                            }
                        }
                    }
                }
            }

            else {
                if (gridMouseX >= 0 and gridMouseX <= gridWidth-1 and gridMouseY >= 0 and gridMouseY <= gridHeight-1){
                    if (grid[gridMouseX][gridMouseY].id > 0) {
                        grid[gridMouseX][gridMouseY] = particle();
                        grid[gridMouseX][gridMouseY].id = 0;
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        BeginDrawing();

            ClearBackground({100, 100, 100, 255});

            for (int x=0; x < gridWidth; x++) {
                DrawLine(x*cellSize, 0, x*cellSize, screenHeight, {120, 120, 120, 127});
            }
            for (int y=0; y < gridHeight; y++) {
                DrawLine(0, y*cellSize, screenWidth, y*cellSize, {120, 120, 120, 127});
            }

            // for (int x=0; x < gridWidth; x++) {
                // for (int y=0; y < gridHeight; y++) {
            for (int x=gridWidth-1; x >= 0; x--) {
                for (int y=gridHeight; y >= 0; y--) {
                    if (grid[x][y].hasBeenUpdated == false){
                        switch (grid[x][y].id)
                        {
                        case 1:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            updateSand(x, y, grid);
                            break;
                        
                        case 4:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            updateSand(x, y, grid);
                            break;

                        case 2:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            updateWater(x, y, grid);
                            break;
                        
                        case 3:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            break;
                        
                        case 5:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            break;
                        
                        case 6:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            updateFire(x, y, grid);
                            break;

                        case -1:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, {20, 20, 20, 255});
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
            for (int x=0; x < gridWidth; x++) {
                for (int y=0; y < gridHeight; y++) {
                    if (grid[x][y].id != -1) {
                        grid[x][y].hasBeenUpdated = false;
                    }
                }
            }

            Vector2 previewPos = {(gridMouse.x*cellSize)-brushSize/2*cellSize, (gridMouse.y*cellSize)-brushSize/2*cellSize};
            DrawRectangleLines(previewPos.x, previewPos.y, brushSize*cellSize, brushSize*cellSize, {255 ,0 ,0 ,255});

            DrawText("Falling sand simulation", 10, 10, 20, {20, 20, 10, 255});
            DrawText(TextFormat("Grid int:    %i : %i",gridMouseX,gridMouseY), 10, 60, 20, {20, 20, 10, 255});
            DrawText(TextFormat("Grid mouse:  %f : %f",gridMouse.x,gridMouse.y), 10, 40, 20, {20, 20, 20, 255});
            DrawText(TextFormat("World mouse: %f : %f",worldMouse.x,worldMouse.y), 10, 80, 20, {20, 20, 20, 255});
            DrawText(TextFormat("mouse pos:   %f : %f",mouse.x,mouse.y), 10, 100, 20, {20, 20, 20, 255});
            DrawText(TextFormat("Brush size:  %i", brushSize), 10, 120, 20, {20, 20, 20, 255});

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}


