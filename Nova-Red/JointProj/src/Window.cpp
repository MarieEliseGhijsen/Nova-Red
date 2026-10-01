#include "../include/Window.h"

Window::Window() : m_window(sf::VideoMode({ s_screenWidth, s_screenHeight }), "Game")
{
	m_window.setVerticalSyncEnabled(true);

	//font
	if (!m_font.openFromFile("Resources\\ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "problem loading arial black font in window.cpp" << std::endl;
	}
}

std::optional<sf::Event> Window::pollEvent()
{
	return m_window.pollEvent();
}

void Window::renderGameRunning(const Player& t_player, const Level& t_level)
{
	m_window.clear();

	renderWalls(t_player, t_level);
	renderMiniMap(t_player, t_level);

	m_window.display();
}

void Window::renderWalls(const Player& t_player, const Level& t_level)
{
	int screenWidth = m_window.getSize().x;
	int screenHeight = m_window.getSize().y;

	m_depthBuffer.resize(screenWidth);
	m_floorBuffer.resize(screenWidth);

	auto pos = t_player.getPosition();
	float angle = t_player.getAngle();

	float rayAngle = 0.0f;

	float distanceToWall = 0.0f;
	bool hitWall = false;

	float eyeX = 0.0f;
	float eyeY = 0.0f;

	int testX = 0;
	int testY = 0;

	int ceiling = 0;
	int floor = 0;

	int shade = 0;

	for (int x = 0; x < screenWidth; x++) //each vertical slice of wall
	{
		//calculate ray angle for this slice
		rayAngle = (angle - m_FOV / 2.0f) + ((float)x / screenWidth) * m_FOV;

		distanceToWall = 0.0f;
		hitWall = false;

		//direction vector for ray
		eyeX = cos(rayAngle);
		eyeY = sin(rayAngle);

		//step forward along ray until wall is hit or max depth reached
		while (!hitWall && distanceToWall < m_maxDepth)
		{
			distanceToWall += 0.05f;

			testX = (int)(pos.x + eyeX * distanceToWall);
			testY = (int)(pos.y + eyeY * distanceToWall);

			if (t_level.getTileType(testX, testY) == 1) //check if ray hit a wall
			{
				hitWall = true;
			}
		}

		distanceToWall *= cos(rayAngle - angle); //correct distortion
		m_depthBuffer[x] = distanceToWall; //store distance in depth buffer

		//calculate wall height on screen
		ceiling = (screenHeight / 2.0) - screenHeight / distanceToWall;
		floor = screenHeight - ceiling;
		m_floorBuffer[x] = floor; //store floor in floor buffer

		shade = 255 - (distanceToWall * 20); //shading based on distance
		shade = std::max(0, shade);

		//create verticle slice
		sf::RectangleShape wall;
		wall.setSize({ 1, (float)(floor - ceiling) });
		wall.setPosition({ (float)x, (float)ceiling });
		wall.setFillColor(sf::Color(shade, shade, shade));

		m_window.draw(wall);
	}
}

void Window::renderMiniMap(const Player& t_player, const Level& t_level)
{
	auto playerPos = t_player.getPosition();

	sf::RectangleShape tempRect;
	tempRect.setSize(sf::Vector2f{ 5, 5 });
	tempRect.setFillColor(sf::Color::Red);

	for (int col = 0; col < t_level.getHeight(); col++)
	{
		for (int row = 0; row < t_level.getWidth(); row++)
		{
			//draw tiles first
			if (t_level.getTileType(row, col) == 1)
			{
				tempRect.setFillColor(sf::Color::Red);
				tempRect.setPosition({ 20.f + row * 5.f, 20.f + col * 5.f });
				m_window.draw(tempRect);
			}

			//draw player
			if (row == (int)playerPos.x && col == (int)playerPos.y)
			{
				tempRect.setFillColor(sf::Color::Cyan);
				tempRect.setPosition({ 20.f + row * 5.f, 20.f + col * 5.f });
				m_window.draw(tempRect);
			}
		}
	}
}

bool Window::isOpen() const
{
	return m_window.isOpen();
}

void Window::close()
{
	m_window.close();
}

sf::RenderWindow &Window::getWindow()
{
	return m_window;
}