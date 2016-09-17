#include <SFML/Graphics.hpp>
#include <iostream>

#include "../src/LightSystem.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Lumos", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	sf::View view = window.getDefaultView();

	ltbl::LightSystem ls;
	ls.create({ -1000.f, -1000.f, 2000.f, 2000.f }, window.getSize());

	sf::Texture directionLightTexture;
	directionLightTexture.loadFromFile("LTBL2/examples/directionLightTexture.png");
	directionLightTexture.setSmooth(true);

	sf::Texture pointLightTexture;
	pointLightTexture.loadFromFile("LTBL2/examples/pointLightTexture.png");
	pointLightTexture.setSmooth(true);

	sf::Texture spookyLightTexture;
	spookyLightTexture.loadFromFile("LTBL2/examples/spookyLightTexture.png");
	spookyLightTexture.setSmooth(true);

	//----- Add Lights and Blocks

	auto sun = ls.createLightPointDirection();
	sun->setOrigin(sf::Vector2f(directionLightTexture.getSize().x * 0.5f, directionLightTexture.getSize().y * 0.5f));
	sun->setTexture(directionLightTexture);
	sun->setScale(sf::Vector2f(6.0f, 6.0f));
	sun->setColor(sf::Color(255, 230, 200));

	auto mlight = ls.createLightPointEmission();
	mlight->setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
	mlight->setTexture(pointLightTexture);
	mlight->setScale(1.f, 1.f);
	mlight->setColor(sf::Color::White);

	std::vector<sf::RectangleShape> shapes;

	sf::RectangleShape blocker;
	blocker.setSize({ 200.f, 50.f });
	blocker.setPosition({ 500.f, 300.f });
	blocker.setFillColor(sf::Color::Red);

	shapes.push_back(blocker);

	auto lightBlocker = ls.createLightShape();
	lightBlocker->setPointCount(4u);
	lightBlocker->setPoint(0u, { 0.f, 0.f });
	lightBlocker->setPoint(1u, { 0.f, blocker.getSize().y });
	lightBlocker->setPoint(2u, blocker.getSize());
	lightBlocker->setPoint(3u, { blocker.getSize().x, 0.f });
	lightBlocker->setPosition(blocker.getPosition());

	float angle = 0.f;

	sf::Vector2f beginRight, endRight;

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle)
			{
				if (mlight != nullptr)
				{
					mlight->remove(); // or : ls.removeLight(mlight);
					mlight = nullptr;
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
			{
				beginRight = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }, view);
			}
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
			{
				endRight = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }, view);

				sf::RectangleShape blocker;
				blocker.setSize(endRight - beginRight);
				blocker.setPosition(beginRight);
				blocker.setFillColor(sf::Color::Red);

				shapes.push_back(blocker);

				auto lightBlocker = ls.createLightShape();
				lightBlocker->setPointCount(4u);
				lightBlocker->setPoint(0u, { 0.f, 0.f });
				lightBlocker->setPoint(1u, { 0.f, blocker.getSize().y });
				lightBlocker->setPoint(2u, blocker.getSize());
				lightBlocker->setPoint(3u, { blocker.getSize().x, 0.f });
				lightBlocker->setPosition(blocker.getPosition());
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				auto light = ls.createLightPointEmission();
				light->setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
				light->setTexture(pointLightTexture);
				light->setScale(1.f, 1.f);
				light->setColor(sf::Color::White);
				light->setPosition(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }, view));
			}
		}

		sf::Time dt = clock.restart();

		angle += 10.f * dt.asSeconds();
		sun->setCastAngle(angle);

		if (mlight != nullptr)
		{
			mlight->setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window), view));
		}

		sf::Vector2f mvt;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			mvt.y--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			mvt.y++;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			mvt.x--;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			mvt.x++;
		view.move(mvt * dt.asSeconds() * 200.f);

		window.clear(sf::Color::White);
		window.setView(view);

		for (std::size_t i = 0; i < shapes.size(); i++)
		{
			window.draw(shapes[i]);
		}

		ls.render(window);

		window.display();
	}

	return 0;
}