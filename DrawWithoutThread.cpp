#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

namespace DrawWithoutThread {

    int main() {
        RenderWindow window(VideoMode(800, 600), "Drawing App");
        window.setFramerateLimit(60); // Set the frame rate limit

        CircleShape brush(5); // Create a brush (circle shape) for drawing
        brush.setFillColor(Color::White); // Set brush color

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                // Handle other events here
            }

            // Your drawing logic here
            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i mousePosition = Mouse::getPosition(window);
                brush.setPosition(Vector2f(mousePosition));
                window.draw(brush);
            }

            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                window.clear();
            }

            // Draw your shapes here using window.draw() function
            window.display();  // Display the updated window
        }
        return 0;
    }
}