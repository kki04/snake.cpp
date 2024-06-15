#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

// 맵 크기 정의
const int HEIGHT = 21;
const int WIDTH = 21;
const int TILE_SIZE = 20;

// 아이템 타입 정의
const int GROWTH_ITEM = 1;
const int POISON_ITEM = 2;

// Snake 구조체 정의
struct Snake {
    int y, x;
    Snake(int y, int x) : y(y), x(x) {}
};

// 아이템 구조체 정의
struct Item {
    int y, x, type;
    Item(int y, int x, int type) : y(y), x(x), type(type) {}
};

// Snake 초기화 함수
void initSnake(std::vector<Snake>& snake) {
    snake.push_back(Snake(10, 10)); // Head
    snake.push_back(Snake(10, 9));  // Body
    snake.push_back(Snake(10, 8));  // Body
}

// 아이템 생성 함수
void generateItem(std::vector<Item>& items, const std::vector<std::vector<int>>& map, int type) {
    int y, x;
    do {
        y = rand() % (HEIGHT - 2) + 1;
        x = rand() % (WIDTH - 2) + 1;
    } while (map[y][x] != 0);
    items.push_back(Item(y, x, type));
}

// Snake 이동 함수
void moveSnake(std::vector<Snake>& snake, int direction) {
    int dx = 0, dy = 0;
    switch (direction) {
        case 0: dy = -1; break; // Up
        case 1: dy = 1; break;  // Down
        case 2: dx = -1; break; // Left
        case 3: dx = 1; break;  // Right
    }

    // 새 위치 계산
    int newY = snake[0].y + dy;
    int newX = snake[0].x + dx;

    // 몸통 따라가기
    for (int i = snake.size() - 1; i > 0; --i) {
        snake[i] = snake[i - 1];
    }
    // 새로운 머리 위치
    snake[0] = Snake(newY, newX);
}

// 아이템 처리 함수
void processItems(std::vector<Snake>& snake, std::vector<Item>& items) {
    int headY = snake[0].y;
    int headX = snake[0].x;

    for (auto it = items.begin(); it != items.end();) {
        if (it->y == headY && it->x == headX) {
            if (it->type == GROWTH_ITEM) {
                snake.push_back(Snake(snake.back().y, snake.back().x)); // 성장
            } else if (it->type == POISON_ITEM) {
                if (snake.size() > 1) {
                    snake.pop_back(); // 축소
                }
            }
            it = items.erase(it);
        } else {
            ++it;
        }
    }
}

// 충돌 확인 함수
bool isCollision(const std::vector<Snake>& snake, const std::vector<std::vector<int>>& map) {
    int headY = snake[0].y;
    int headX = snake[0].x;

    // 벽과 충돌 확인
    if (map[headY][headX] == 1) {
        return true;
    }

    // 자기 몸과 충돌 확인
    for (int i = 1; i < snake.size(); ++i) {
        if (snake[i].y == headY && snake[i].x == headX) {
            return true;
        }
    }

    return false;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // SFML 윈도우 생성
    sf::RenderWindow window(sf::VideoMode(WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE), "Snake Game");

    // Snake 초기화
    std::vector<Snake> snake;
    initSnake(snake);
    std::vector<Item> items;

    // 맵 초기화용 2차원 벡터 생성
    std::vector<std::vector<int>> map(HEIGHT, std::vector<int>(WIDTH, 0));
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                map[i][j] = 1;  // 벽
            }
        }
    }

    // 아이템 생성
    generateItem(items, map, GROWTH_ITEM);
    generateItem(items, map, POISON_ITEM);

    int direction = 3; // 초기 방향 (오른쪽)
    int itemSpawnCounter = 0; // 아이템 생성 타이머

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up && direction != 1) direction = 0;
                if (event.key.code == sf::Keyboard::Down && direction != 0) direction = 1;
                if (event.key.code == sf::Keyboard::Left && direction != 3) direction = 2;
                if (event.key.code == sf::Keyboard::Right && direction != 2) direction = 3;
            }
        }

        // Snake 이동
        moveSnake(snake, direction);

        // 아이템 처리
        processItems(snake, items);

        // 충돌 확인
        if (isCollision(snake, map)) {
            window.close();
            break;
        }

        // 아이템 주기적으로 생성
        itemSpawnCounter++;
        if (itemSpawnCounter >= 35) { // 35틱마다 아이템 생성 (약 7초)
            itemSpawnCounter = 0;
            generateItem(items, map, (rand() % 2 == 0) ? GROWTH_ITEM : POISON_ITEM);
        }

        window.clear();

        // 맵 그리기
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);

                if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                    tile.setFillColor(sf::Color::White); // 벽 색상
                } else {
                    tile.setFillColor(sf::Color::Black); // 빈 공간 색상
                }

                window.draw(tile);
            }
        }

        // Snake 그리기
        for (const auto& s : snake) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(s.x * TILE_SIZE, s.y * TILE_SIZE);
            tile.setFillColor(sf::Color::Green); // Snake 색상
            window.draw(tile);
        }

        // 아이템 그리기
        for (const auto& item : items) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(item.x * TILE_SIZE, item.y * TILE_SIZE);

            if (item.type == GROWTH_ITEM) {
                tile.setFillColor(sf::Color::Blue); // 성장 아이템 색상
            } else if (item.type == POISON_ITEM) {
                tile.setFillColor(sf::Color::Red); // 독 아이템 색상
            }

            window.draw(tile);
        }

        window.display();
        sf::sleep(sf::milliseconds(200)); // 200ms 대기
    }

    return 0;
}