#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <iostream>

#define PI 3.14159

const int WIDTH = 1000;
const int HEIGHT = 600;

bool map2d_visible = false;
bool lines2d_visible = false;
bool player2d_visible = false;
bool level_visible = true;

sf::Vector2f player_pos(WIDTH / 2, HEIGHT / 2);
float player_angle = 0;



std::vector <std::vector <int> > map
{
	{1,1,1,1,1,1,1,1,1},
	{1,0,1,0,0,1,0,0,1},
	{1,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,1,0,0,1},
	{1,1,0,0,0,0,0,0,1},
	{1,0,1,1,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1}
};


float deg_to_rad(float deg) {
	return 2 * PI * deg / 360;
}



void drawLines(sf::RenderWindow& window, sf::Vector2f player_pos, float player_angle) {
	int num_rays = 200;
	float FOV = 180.f;
	float max_depth = WIDTH;
	float delta_angle = 2 * PI * ((FOV / num_rays) / 360);
	float rad = deg_to_rad(player_angle);

	float cur_angle = rad - deg_to_rad(90 -(FOV / 2));

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

			if(lines2d_visible)
				window.draw(line, 2, sf::Lines);

			int block_x = (int)(ray_x / WIDTH * map.size());
			int block_y = (int)(ray_y / HEIGHT * map[0].size());
			if (map[block_y][block_x] == 1)
			{




				float dist = num_rays / 2 * -tan(FOV / 2);
				float proj_coeff = dist * 100;
				float scale = WIDTH / num_rays;
				float proj_height = proj_coeff / depth;

				sf::RectangleShape line(sf::Vector2f(proj_height, scale));
				int color = 225 - (225 / max_depth * depth);
				line.setFillColor(sf::Color(color/5, color, color/2,-1));
				line.setPosition(WIDTH - scale*i, 300 - proj_height/2);
				line.rotate(90);
				
				if(level_visible)
					window.draw(line);


				break;
			}
		}
		cur_angle -= delta_angle;
	}

	
}



void drawPlayer(sf::RenderWindow &window)
{
	sf::CircleShape player (10.f,3);
	player.setFillColor(sf::Color::Green);
	player.setPosition(player_pos);
	player.setRotation(player_angle);
	player_angle = player.getRotation();

	drawLines(window, player_pos, player.getRotation());

	if(player2d_visible)
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) { player_angle += 3; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { player_angle -= 3; }

}

void drawMap2D(sf::RenderWindow& window)
{
	for (int y = 0; y < map.size(); y++)
	{
		for (int x = 0; x < map[y].size(); x++)
		{
			if (map[y][x] == 1) {
				sf::RectangleShape map_block(sf::Vector2f( WIDTH / map[y].size(), HEIGHT / map.size()));
				map_block.setFillColor(sf::Color::Blue);
				map_block.setPosition(WIDTH / map[y].size() * x+1, HEIGHT / map.size() * y+1);

				if(map2d_visible)
					window.draw(map_block);
			}
		}
	}
}

void drawSky(sf::RenderWindow& window)
{
	sf::RectangleShape map_block(sf::Vector2f(WIDTH, HEIGHT/2));
	map_block.setFillColor(sf::Color::Blue);
	map_block.setPosition(0,0);
	window.draw(map_block);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RT");
	
	//window.setFramerateLimit(60);


	float fps;
	sf::Clock clock = sf::Clock::Clock();
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;

	



	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{			if (event.type == sf::Event::Closed)
			window.close();
		}
		window.clear(sf::Color(50,50,50,-1));

		//drawSky(window);
		drawMap2D(window);

		drawPlayer(window);
		buttons();



		currentTime = clock.getElapsedTime();
		fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
		std::cout << "fps =" << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
		previousTime = currentTime;


		window.display();
	}

}