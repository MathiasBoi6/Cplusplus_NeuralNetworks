#include <SFML/Graphics.hpp>
#include <iostream>
#include "algo.h"
#include "grid.h"
#include <vector>
#include "windowDefaults.h"
#include "LoadImages.h"
#include <iomanip>
using namespace sf;



int main() {
	NeuralNetwork::NeuralNetwork nn = MakeImageNN();
	std::cout << "returned from nnCreation\n";

	RenderWindow window(VideoMode(width, height), "Drawing App");
	window.setFramerateLimit(60); // Set the frame rate limit
	bool updateWindow = true;
	bool predictReady = false;

	

	std::vector<VertexArray> backgroundGrid = grid::SetupGrid(gridSize);
	std::vector<std::vector<Vector2i>> gridCenters = grid::gridCentersArr(gridSize);
	std::vector<std::vector<double>> gridValues(gridSize, std::vector < double>(gridSize, 0.0));

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
			Vector2i gridPosition = grid::GetTile(mousePosition);
			Vector2i gridCenter = gridCenters[gridPosition.x][gridPosition.y];
			//std::cout << gridPosition.x << " and " << gridPosition.y << "\n";

			float distFromMid = grid::GetCenterDist(mousePosition, gridCenter);
			if (distFromMid > gridValues[gridPosition.x][gridPosition.y]) {
				gridValues[gridPosition.x][gridPosition.y] = distFromMid;
			}
			updateWindow = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			//Reset grid values
			for (int i = 0; i < gridSize; ++i) {
				for (int j = 0; j < gridSize; ++j) {
					gridValues[i][j] = 0.0f;
				}
			}
			updateWindow = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::P) && predictReady) {
			predictReady = false;
			std::cout << "p was pressed\n";
			std::cout << gridValues.size() << "\n";
			

			std::vector<double> flattenedGrid;
			for (const auto& row : gridValues) {
				flattenedGrid.insert(flattenedGrid.end(), row.begin(), row.end());
			}
			std::cout << flattenedGrid.size() << "\n";
			std::vector<double> prediction = nn.predict(flattenedGrid);

			std::cout << std::fixed << std::setprecision(3);
			std::cout << "Predicted: (";
			for (size_t i = 0; i < prediction.size(); ++i) {
				std::cout << prediction[i] << ", ";
			}
			std::cout << ")\n";
		}

		if (updateWindow) {
			window.clear();

			for (VertexArray line : backgroundGrid) {
				window.draw(line);
			}

			//Draw squares
			int stepSizeX = width / gridSize;
			int stepSizeY = height / gridSize;
			RectangleShape square(Vector2f(width / gridSize, height / gridSize));
			for (int i = 0; i < gridSize; ++i) {
				for (int j = 0; j < gridSize; ++j) {
					square.setFillColor(Color(255, 0, 0, gridValues[i][j] * 255.0f));

					square.setPosition(stepSizeX * i, stepSizeY * j);
					window.draw(square);
				}
			}



			window.display();
			updateWindow = false;
			predictReady = true;
		}
	}

	return 0;
}
/* PRINT GRID CENTERS

for (int i = 0; i < gridSize; ++i) {
	for (int j = 0; j < gridSize; ++j) {
		std::cout << gridCenters[i][j].x << " and " << gridCenters[i][j].y << " | ";
	}
	std::cout << "\n";
}
std::cout << "\n";
*/