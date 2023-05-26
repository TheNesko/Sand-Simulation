#include "raylib.h"
#include "particle.h"
#include "utils.h"
#include "grid.h"
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


int main(void)
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Game");

    std::vector<std::vector <particle>> grid;
    grid.resize(gridWidth);
    for (int x=0; x < gridWidth; x++){
        grid[x].resize(gridHeight);
    }

    generateBorders(grid, gridWidth, gridHeight);
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
                            updateSand(x, y, grid, gridWidth, gridHeight);
                            break;
                        
                        case 4:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            updateSand(x, y, grid, gridWidth, gridHeight);
                            break;

                        case 2:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            updateWater(x, y, grid, gridWidth, gridHeight);
                            break;
                        
                        case 3:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            break;
                        
                        case 5:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            break;
                        
                        case 6:
                            DrawRectangleV(getWorldPosition({(float)x, (float)y}, cellSize), {cellSize, cellSize}, grid[x][y].color);
                            updateFire(x, y, grid, gridWidth, gridHeight);
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


