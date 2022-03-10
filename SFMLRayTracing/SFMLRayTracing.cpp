#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <iostream>

#define PI 3.14159

const int WIDTH = 600;
const int HEIGHT = 600;

std::vector <std::vector <int> > map
{
	{1,1,1,1,1,1,1,1},
	{1,0,1,0,0,1,0,1},
	{1,0,1,0,0,1,0,1},
	{1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1}
};



sf::Vector2f player_pos(WIDTH / 2, HEIGHT / 2);
float player_angle = 0;

void drawLines(sf::RenderWindow& window, sf::Vector2f player_pos, float player_angle) {
	int num_rays = 100;
	float FOV = 60.f;
	float max_depth = 600.f;
	float delta_angle = 2 * PI * ((FOV / num_rays) / 360);
	float rad = 2 * PI * ((player_angle) / 360);

	float cur_angle = rad - 2 * PI * ((90 -(FOV / 2)) / 360);

	for (int i = 0; i < num_rays; i++)
	{	
		float sin_a = sin(cur_angle);
		float cos_a = cos(cur_angle);
		for (float depth = 0; depth < max_depth; depth++)
		{
			float ray_x = player_pos.x + depth * cos_a;
			float ray_y = player_pos.y + depth * sin_a;

			sf::Vertex line[] =
			{
				sf::Vertex(player_pos),
				sf::Vertex(sf::Vector2f(ray_x, ray_y))
			};

			window.draw(line, 2, sf::Lines);

			int block_x = (int)(ray_x / 600 * 8);
			int block_y = (int)(ray_y / 600 * 8);
			if (map[block_y][block_x] == 1)
			{
				break;
			}
		}
		cur_angle -= delta_angle;
	}

	
}

//void drawLines(sf::RenderWindow& window, sf::Vector2f player_pos, float player_angle) {
//
//	int r, mx, my, mp, dof;
//	float ray_x, ray_y, ray_angle, xo, yo;
//	ray_angle = 2 * 3.14f * (player_angle / 360)+90;
//
//	for (r = 0; r < 1; r++)
//	{
//		dof = 0;
//		float aTan = -1 / tan(ray_angle);
//
//		if (ray_angle > PI) { 
//			ray_y = (((int)player_pos.y >> 6) << 6) - 0.0001;
//			ray_x = (player_pos.y - ray_y) * aTan + player_pos.x;
//			yo = -64;
//			xo = -yo * aTan;
//		}
//		if (ray_angle < PI) {
//			ray_y = (((int)player_pos.y >> 6) << 6) + 64;
//			ray_x = (player_pos.y - ray_y) * aTan + player_pos.x;
//			yo = 64;
//			xo = -yo * aTan;
//		}
//		if (ray_angle == 0 || ray_angle == PI) {
//			ray_x = player_pos.x;
//			ray_y = player_pos.y;
//			dof = 8;
//		}
//		while (dof < 8)
//		{
//			mx = (int)(ray_x) >> 6;
//			my = (int)(ray_y) >> 6;
//			mp = my * mapX + mx;
//			if (mp < mapX * mapY && map[mp] == 1) { dof = 8; }
//			else { ray_x += xo; ray_y += yo; dof += 1; }
//		}
//		sf::Vertex line[] =
//		{
//			sf::Vertex(player_pos),
//			sf::Vertex(sf::Vector2f(ray_x, ray_y))
//		};
//
//		window.draw(line, 2, sf::Lines);
//	}
//
//
//	/*sf::RectangleShape line(sf::Vector2f(600.f, 1.f));
//	line.setPosition(player_pos);
//	line.rotate(ray_angle);
//	window.draw(line);*/
//
//}


void drawPlayer(sf::RenderWindow &window)
{
	sf::CircleShape player (10.f,3);
	player.setFillColor(sf::Color::Green);
	player.setPosition(player_pos);
	player.setRotation(player_angle);
	player_angle = player.getRotation();

	drawLines(window, player_pos, player.getRotation());

	window.draw(player);

}



void buttons()
{
	float player_speed = 3.f;
	std::cout << player_angle << std::endl;
	float rad = 2 * PI * (player_angle / 360);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player_pos.x += player_speed * std::cos(rad);
		player_pos.y += player_speed * std::sin(rad);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player_pos.x -= player_speed * std::cos(rad);
		player_pos.y -= player_speed * std::sin(rad);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { 
		player_pos.x += player_speed * std::sin(rad);
		player_pos.y -= player_speed * std::cos(rad); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { 
		player_pos.x -= player_speed * std::sin(rad);
		player_pos.y += player_speed * std::cos(rad);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) { player_angle += 5.55; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { player_angle -= 5.55; }

}

	void drawMap2D(sf::RenderWindow& window)
	{
		std::vector <std::vector <int> > map
		{
			{1,1,1,1,1,1,1,1},
			{1,0,1,0,0,1,0,1},
			{1,0,1,0,0,1,0,1},
			{1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1}
		};

		for (int y = 0; y < map.size(); y++)
		{
			for (int x = 0; x < map[y].size(); x++)
			{
				if (map[y][x] == 1) {
					sf::RectangleShape map_block(sf::Vector2f(600.f / 8.f, 600.f / 8.f));
					map_block.setFillColor(sf::Color::Blue);
					map_block.setPosition(WIDTH / map.size() * x, HEIGHT / map[y].size() * y);
					window.draw(map_block);
				}
			}
		}
	}






int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RT");
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{			if (event.type == sf::Event::Closed)
			window.close();
		}
		window.clear(sf::Color::Black);


		drawMap2D(window);

		drawPlayer(window);
		buttons();

		window.display();
	}

}