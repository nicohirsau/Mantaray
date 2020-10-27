#include "Mantaray/Core/Window.hpp"
#include "Mantaray/Core/KeyCodes.hpp"
#include "Mantaray/Core/InputManager.hpp"
#include "Mantaray/OpenGL/Drawables.hpp"
#include "Mantaray/OpenGL/ObjectLibrary.hpp"

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace MR;

Window* window = nullptr;

enum GameState {
    Playing,
    GameOver,
    Pause
};

GameState gameState = GameState::Playing;

float snakeStepTime = .25f;
float elapsedStepTime = 0;
Vector2i gridSize = Vector2i(20, 18);
Vector2i direction = Vector2i(1, 0);
std::vector<Vector2i> snakeNodes = std::vector<Vector2i>({
    Vector2i(9, 8),
    Vector2i(10, 8),
    Vector2i(11, 8)
});
std::vector<Vector2i> leftOvers = std::vector<Vector2i>();

Vector2i applePosition = Vector2i(1, 1);

inline void ProcessInput(float deltaTime) {
    if (InputManager::GetKeyDown(MR_KEY_ESCAPE)) window->setShouldClose();

    if (gameState != GameOver) {
        if (InputManager::GetKeyDown(MR_KEY_SPACE)) gameState = (gameState == Playing) ? Pause : Playing;   
    }

    if (gameState == Playing) {
        if (InputManager::GetKeyDown(MR_KEY_W)) direction = Vector2i( 0,  1);
        if (InputManager::GetKeyDown(MR_KEY_A)) direction = Vector2i(-1,  0);
        if (InputManager::GetKeyDown(MR_KEY_S)) direction = Vector2i( 0, -1);
        if (InputManager::GetKeyDown(MR_KEY_D)) direction = Vector2i( 1,  0);
    }
}

inline void Update(float deltaTime) {
    // Check if gameover
    for (int i = snakeNodes.size() - 2; i >= 0; i--) {
        if (snakeNodes[snakeNodes.size()-1] == snakeNodes[i]) {
            std::cout << "GameOver" << std::endl;
            gameState = GameState::GameOver;
            return;
        }
    }
    Vector2i snakeHeadPosition = snakeNodes[snakeNodes.size() - 1];
    if (snakeHeadPosition.x < 0 || snakeHeadPosition.x > gridSize.x - 1 ||
        snakeHeadPosition.y < 0 || snakeHeadPosition.y > gridSize.y - 1) {
            std::cout << "GameOver" << std::endl;
            gameState = GameState::GameOver;
            return;
    }

    // Step forward if elapsed time is greater step time
    elapsedStepTime += deltaTime;
    if (elapsedStepTime >= snakeStepTime) {
        snakeNodes.push_back(snakeNodes[snakeNodes.size() - 1] + direction);
        snakeNodes.erase(snakeNodes.begin());
        elapsedStepTime = 0;
    }

    // Place leftover if snake ate an apple
    if (snakeNodes[snakeNodes.size() - 1] == applePosition) {
        leftOvers.push_back(applePosition);
        applePosition = Vector2i(rand() % gridSize.x, rand() % gridSize.y);
    }

    // Add new snake node if no snake is over an leftover
    for (int i = leftOvers.size() - 1; i >= 0; i--) {
        bool snakeOverLeftOver = false;
        for (Vector2i snakeNode : snakeNodes) {
            if (snakeNode == leftOvers[i]) {
                snakeOverLeftOver = true;
                break;
            }
        }
        if (!snakeOverLeftOver) {
            snakeNodes.insert(snakeNodes.begin(), leftOvers[i]);
            leftOvers.erase(leftOvers.begin() + i);
        }
    }
} 

int main()
{
    srand(time(NULL));

    window = Window::CreateWindow("Snake", Vector2u(320, 288), Vector2u(160, 144), Vector2f(gridSize.x, gridSize.y));
    window->setClearColor(MR::Color(48, 98, 48));
    
    InputManager::AddKeyToWatch(MR_KEY_ESCAPE);
    InputManager::AddKeyToWatch(MR_KEY_SPACE);

    InputManager::AddKeyToWatch(MR_KEY_W);
    InputManager::AddKeyToWatch(MR_KEY_A);
    InputManager::AddKeyToWatch(MR_KEY_S);
    InputManager::AddKeyToWatch(MR_KEY_D);
    
    Texture* tileSheet = ObjectLibrary::CreateTexture("spriteSheet", "Content/snake.png");

    Sprite snakeNodeSprite = Sprite(tileSheet);
    snakeNodeSprite.absoluteSize = true;
    snakeNodeSprite.sourceRectangle = Rectanglef(0, .75f, .25f, .25f);

    Sprite appleSprite = Sprite(tileSheet);
    appleSprite.absoluteSize = true;
    appleSprite.sourceRectangle = Rectanglef(.25f, .75f, .25f, .25f);

    float deltaTime;
    while (!window->getShouldClose()) {
        deltaTime = window->update();

        ProcessInput(deltaTime);
        switch (gameState) {
            case GameState::Playing:
                Update(deltaTime);
                break;
            case GameState::Pause:
                break;
            case GameState::GameOver:
                break;
            default:
                break;
        }

        window->beginFrame();
        for (Vector2i& snakeNodePosition : snakeNodes) {
            snakeNodeSprite.position = Vector2f(snakeNodePosition.x, snakeNodePosition.y);
            window->draw(snakeNodeSprite);
        }
        appleSprite.position = Vector2f(applePosition.x, applePosition.y);
    	window->draw(appleSprite);
        window->endFrame();
    }

    return 0;
}
