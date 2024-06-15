#include <SFML/Graphics.hpp>

// 맵 크기 정의
const int HEIGHT = 21;
const int WIDTH = 21;
const int TILE_SIZE = 20;

int main() {
    // SFML 윈도우 생성
    sf::RenderWindow window(sf::VideoMode(WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE), "Snake Game");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
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

        window.display();
    }

    return 0;
}