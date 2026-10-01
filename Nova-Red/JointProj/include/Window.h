#pragma once

#include <SFML/Graphics.hpp>

#include "Level.h"

#include "Player.h"

class Window
{
public:
	Window();

	std::optional<sf::Event> pollEvent();

	void renderMenu();
	void renderInstructions();
	void renderSettings();

	void renderGameRunning(const Player& t_player, const Level& t_level);
	void renderMiniMap(const Player& t_player, const Level& t_level);

	void renderWin();
	void renderLose();

	void renderWalls(const Player& t_player, const Level& t_level);

	bool isOpen() const;
	void close();

	sf::RenderWindow &getWindow();
										
private:
	const static int s_screenWidth{ 800 };
	const static int s_screenHeight{ 600 };

	sf::RenderWindow m_window;
	float m_FOV{ 3.14159f / 3.0f };
	float m_maxDepth{ 20.0f };
	std::vector<float> m_depthBuffer;
	std::vector<float> m_floorBuffer;

	sf::Font m_font;
};