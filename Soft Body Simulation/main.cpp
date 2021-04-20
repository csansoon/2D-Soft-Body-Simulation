#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "BodyManager.h"
#include <iostream>
#include <sstream>
using namespace sf;
int fps = 60;
float zoom = 10;
float viewZoom = 1;
float zoomChange = 1.05;
float resolutionWidth = 800.0f, resolutionHeight = 600.0f;
Color backgroundColor = Color(25, 35, 50, 255);


template <typename T>
std::string to_string(T value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

void resizeMap(const RenderWindow& window, View& view) {
	view.setSize(window.getSize().x * viewZoom, window.getSize().y * viewZoom);
}

void resizeView(const RenderWindow& window, View& view) {
	view.setSize(window.getSize().x, window.getSize().y);
	view.setCenter(window.getSize().x / 2, window.getSize().y / 2);
}

int main() {

	/**	Set Window and View	*/

	RenderWindow window(VideoMode(resolutionWidth, resolutionHeight), "Jello");
	window.setFramerateLimit(fps);
	View view(Vector2f(resolutionWidth / 2, resolutionHeight / 2), Vector2f(resolutionWidth, resolutionHeight));
	View windowView(Vector2f(resolutionWidth / 2, resolutionHeight / 2), Vector2f(resolutionWidth, resolutionHeight));
	view.setCenter(0, 0);
	viewZoom = 4.0f;
	resizeMap(window, view);


	/** Load External Assets */
	Font Consolas;
	if (!Consolas.loadFromFile("Assets/Fonts/consola.ttf"))
	{
		std::cout << "Error trying to load \"Assets/Fonts/consola.tff\". Check that the file exists and try again." << std::endl;
		return 0;
	}



	Text coordinates;
	coordinates.setFont(Consolas);
	coordinates.setPosition(0, 0);
	coordinates.setCharacterSize(16);
	/*
	//FONTS
	Font product_sans;
	product_sans.loadFromFile("data/fonts/Product Sans Regular.ttf");
	Font bold_product_sans;
	bold_product_sans.loadFromFile("data/fonts/Product Sans Bold.ttf");
	*/

	BodyManager bodies;
	SoftBody body;
	MassPoint* hoveringMassPoint;
	MassPoint* draggingMassPoint = nullptr;
	FixedBody floor1, floor2;

	floor1.addPoint(-800, 600);
	floor1.addPoint(800, 600);
	floor1.addPoint(800, 1000);
	floor1.addPoint(-800, 1000);

	
	floor2.addPoint(1000, 1000);
	floor2.addPoint(1600, 800);
	floor2.addPoint(1600, 400);
	floor2.addPoint(1800, 400);
	floor2.addPoint(1800, 1200);
	floor2.addPoint(1000, 1400);
	
	
	
	bodies.add(body);
	bodies.add(floor1);
	bodies.add(floor2);

	Vector2i mousePos;
	Vector2i oldMousePos;
	Vector2i windowOrigin;
	mousePos.x = 1;
	mousePos.y = 1;
	oldMousePos.x = 1;
	oldMousePos.y = 1;
	windowOrigin.x = 0;
	windowOrigin.y = 0;
	bool pressing = false;
	bool dragging = false;

	/**	Main Bucle	*/

	while (window.isOpen()) {


		/**	UPDATES */
		mousePos.x = ((double(Mouse::getPosition(window).x) - window.getSize().x / 2) * viewZoom) + (view.getCenter().x);
		mousePos.y = ((double(Mouse::getPosition(window).y) - window.getSize().y / 2) * viewZoom) + (view.getCenter().y);
		windowOrigin.x = view.getCenter().x - view.getSize().x / 2;
		windowOrigin.y = view.getCenter().y - view.getSize().y / 2;

		bodies.update(1.0f / ((float)fps));
		hoveringMassPoint = bodies.getMassPoint(Vector2f(mousePos.x, mousePos.y), 100);

		if (draggingMassPoint != nullptr) {
			draggingMassPoint->lock(Vector2f(mousePos.x, mousePos.y));
		}


		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
			if (event.type == sf::Event::Resized) {
				resizeMap(window, view);
				resizeView(window, windowView);
			}
			if (event.type == Event::KeyPressed) {
				if (Keyboard::isKeyPressed(Keyboard::R)) {
				}
				if (Keyboard::isKeyPressed(Keyboard::I)) {
				}
				if (Keyboard::isKeyPressed(Keyboard::U)) {
				}
				if (Keyboard::isKeyPressed(Keyboard::X)) {
				}
				if (Keyboard::isKeyPressed(Keyboard::Space)) {
				}
				if (Keyboard::isKeyPressed(Keyboard::Add)) {
				}
				if (Keyboard::isKeyPressed(Keyboard::Subtract)) {
				}
			}

			if (event.type == Event::MouseButtonPressed and event.key.code == Mouse::Left) {
				if (not dragging and not pressing) {
					oldMousePos = mousePos;
					pressing = true;
				}
				draggingMassPoint = hoveringMassPoint;
			}
			if (event.type == Event::MouseButtonPressed and event.key.code == Mouse::Right) {
				if (not pressing and not dragging) {
					oldMousePos = mousePos;
					dragging = true;
				}
			}
			if (event.type == Event::MouseButtonReleased and event.key.code == Mouse::Left) {
				if (draggingMassPoint != nullptr) {
					draggingMassPoint->unlock();
					draggingMassPoint = nullptr;
				}
				if (pressing) {
					pressing = false;
				}
			}
			if (event.type == Event::MouseButtonReleased and event.key.code == Mouse::Right) {
				if (dragging) dragging = false;
				if (draggingMassPoint != nullptr) {
					draggingMassPoint = nullptr;
				}
			}

			if (event.type == Event::MouseWheelMoved) {
				if (event.mouseWheel.delta > 0) {
					viewZoom /= zoomChange;
					resizeMap(window, view);
				}
				else if (event.mouseWheel.delta < 0) {
					viewZoom *= zoomChange;
					resizeMap(window, view);
				}
			}
		}


		/**	Graphics	*/

		if (dragging) {
			view.setCenter((view.getCenter().x) + (oldMousePos.x) - (mousePos.x), (view.getCenter().y) + (oldMousePos.y) - (mousePos.y));
		}

		window.clear(backgroundColor);
		window.setView(view);


		bodies.print(window);


		//window.draw(background);

		/*
		if (pressing) {
			VertexArray line1(LinesStrip, 2);
			line1[1].position = Vector2f(mousePos.x, mousePos.y);
			line1[0].position = Vector2f(oldMousePos.x, oldMousePos.y);
			window.draw(line1);
		}
		*/

		//Text zoomText(to_string(zoom),product_sans,16);


		// UI:
		window.setView(windowView);

		//HUD
		coordinates.setString("(" + to_string(mousePos.x) + ", " + to_string(mousePos.y) + ")");
		window.draw(coordinates);

		window.display();
	}
	return EXIT_SUCCESS;
}

void print() {

}
