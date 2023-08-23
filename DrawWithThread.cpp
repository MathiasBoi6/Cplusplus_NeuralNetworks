#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
using namespace sf;

namespace DrawWithThread {

    template <typename T>
    T PopBetter(std::queue<T>& q) {
        T frontElement = q.front();
        q.pop();
        return frontElement;
    }

    void MouseInput(RenderWindow& window, std::queue<Vector2i>& mousePositionQueue, std::mutex& mousePositionMutex) {
        while (window.isOpen()) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                std::lock_guard<std::mutex> lock(mousePositionMutex);
                mousePositionQueue.push(Mouse::getPosition(window));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }


    int main() {
        RenderWindow window(VideoMode(800, 600), "Drawing App");
        window.setFramerateLimit(60); // Set the frame rate limit

        CircleShape brush(5); // Create a brush (circle shape) for drawing
        brush.setFillColor(Color::White); // Set brush color
        std::queue<Vector2i> mousePositionQueue;
        bool isDrawing = false;
        Vector2i lastPosition;

        std::mutex mousePositionMutex;
        std::thread mouseThread(MouseInput, std::ref(window), std::ref(mousePositionQueue), std::ref(mousePositionMutex));

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
                std::lock_guard<std::mutex> lock(mousePositionMutex);
                if (!mousePositionQueue.empty()) { 
                    //Ensure that there is at least one input
                    if (!isDrawing) {
                        isDrawing = true;
                        lastPosition = PopBetter(mousePositionQueue);
                    }

                    while (!mousePositionQueue.empty()) {
                        Vector2i mousePosition = PopBetter(mousePositionQueue);
                        Vector2f interpolatedPosition = Vector2f(mousePosition) + Vector2f(mousePosition - lastPosition) * 0.1f;
                        brush.setPosition(interpolatedPosition);
                    
                        window.draw(brush);

                        if (mousePositionQueue.empty()) {
                            lastPosition = mousePosition;
                        }
                    }
                }
            }
            else {
                isDrawing = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                window.clear();
            }

            // Draw your shapes here using window.draw() function

            window.display();  // Display the updated window
        }
        mouseThread.join();
        return 0;
    }

}