#pragma once
#pragma warning(suppress : 26451)
#include <SFML/Graphics.hpp>
#include "windowDefaults.h"

namespace grid {
    std::vector<sf::VertexArray> SetupGrid(int n) {
        std::vector<sf::VertexArray> verArr((n - 1) * 2, sf::VertexArray(sf::LineStrip, 2));
        for (int i = 0; i < n - 1; ++i) {
            int xCoord = (width / n) * (i + 1);
            int yCoord = (height / n) * (i + 1);
            verArr[i][0].position = sf::Vector2f(xCoord, 0);
            verArr[i][1].position = sf::Vector2f(xCoord, height);
            verArr[i + (n - 1)][0].position = sf::Vector2f(0, yCoord);
            verArr[i + (n - 1)][1].position = sf::Vector2f(width, yCoord);
        }
        return verArr;
    }

	sf::Vector2i GetTile(sf::Vector2i pos) {

		pos.x = algo::clamp(pos.x * gridSize / width, 0, gridSize - 1);
		pos.y = algo::clamp(pos.y * gridSize / height, 0, gridSize - 1);

		return pos;
	}

	float GetCenterDist(sf::Vector2i mouse, sf::Vector2i center) {
		float radiusX = (width / gridSize) / 2;
		float radiusY = (height / gridSize) / 2;
		sf::Vector2f temp = sf::Vector2f(mouse - center);
		//std::cout << temp.x << " and " << temp.y << "\n";
		return 1.0f - (abs(temp.x) + abs(temp.y)) / (radiusX + radiusY);
	}

	std::vector<std::vector<sf::Vector2i>> gridCentersArr(int n) {
		std::vector<std::vector<sf::Vector2i>> gridArr(n, std::vector<sf::Vector2i>(n));;
		int xSpace = (width / n);
		int ySpace = (height / n);

		for (int i = 0; i < n; ++i) {
			int xmid = xSpace * (i)+xSpace / 2;
			for (int j = 0; j < n; ++j) {
				int ymid = ySpace * (j)+ySpace / 2;
				gridArr[i][j] = sf::Vector2i(xmid, ymid);
			}
		}

		return gridArr;
	}
}