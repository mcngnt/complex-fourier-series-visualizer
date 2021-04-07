#include <SFML/Graphics.hpp>
#include "CFS.h"
// 

void centerText(sf::Text* t_)
{
    sf::FloatRect textRect = t_->getLocalBounds();
    t_->setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top  + textRect.height/2.0f);
}

int main()
{
	srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H, 32), "Main", sf::Style::Default);
    // sf::RenderTexture saveBuffer;
    // saveBuffer.create(SCREEN_W, SCREEN_H);
    window.setFramerateLimit(120);


    int tick = 0;
    int sampleNB = 600;
    std::string currentFileName = "fourier.txt";
    CFS cfs;
    float currentT;
    float pointsDuration[sampleNB] = {0};
    cfs.generate_target_eq(currentFileName);
    cfs.calculate_coef();
    cfs.sort_coef();

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                cfs.circlesNB++;
                cfs.calculate_coef();
                cfs.sort_coef();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                cfs.circlesNB--;
                cfs.calculate_coef();
                cfs.sort_coef();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            {
                std::string fileName;
                std::cout << "Type name of the save (batman.txt for example) : ";
                std::cin >> fileName;
                cfs.generate_target_eq(fileName);
                cfs.calculate_coef();
                cfs.sort_coef();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                std::string circNB;
                std::cout << "Type the number of epicycloids : ";
                std::cin >> circNB;
                
                cfs.circlesNB = abs(std::stoi(circNB));
                cfs.calculate_coef();
                cfs.sort_coef();
            }
        }

        tick++;
        currentT = (float)(tick%CYCLE_DURATION)/(float)CYCLE_DURATION;
        window.clear();
        // saveBuffer.clear();
        sf::VertexArray points(sf::Points, sampleNB);
        sf::VertexArray line(sf::Lines, 2);
        sf::Vector2f currentPos = vec_to_sfvec2_v(cfs.world_to_screen_pos(vec_v(0,0)));
        sf::Vector2f previousPos = currentPos;
        sf::CircleShape circ;
        circ.setFillColor(sf::Color::Transparent);
        sf::Color outlineCircCol = sf::Color::Green;
        circ.setOutlineThickness(1);
        std::complex<float> sum(0,0);
        line[0].color.a = 70;
        line[1].color.a = 70;
        for (int i = 0; i < cfs.circlesNB; ++i)
        {
            sum += cfs.apply_cfs_equation_at_n(currentT, i);
            previousPos = currentPos;
            currentPos = vec_to_sfvec2_v(cfs.world_to_screen_pos(vec_v(real(sum), imag(sum))));
            line[0].position = previousPos;
            line[1].position = currentPos;
            circ.setPosition(previousPos);
            circ.setRadius(sqrt((previousPos.x-currentPos.x)*(previousPos.x-currentPos.x) + (previousPos.y-currentPos.y)*(previousPos.y-currentPos.y)));
            circ.setOrigin(circ.getRadius(), circ.getRadius());
            outlineCircCol.a = (float)i/(float)cfs.circlesNB * 255.f + 50.f;
            circ.setOutlineColor(outlineCircCol);
            window.draw(line);
            // saveBuffer.draw(line);
            window.draw(circ);
            // saveBuffer.draw(circ);
        }
        pointsDuration[(int)(currentT * (float)(sampleNB-1))] = 1.f;

        currentPos = vec_to_sfvec2_v(cfs.world_to_screen_pos(vec_v(real(cfs.apply_cfs_equation(0)),imag(cfs.apply_cfs_equation(0)))));
        previousPos = currentPos;
        for (int i = 0; i < sampleNB; ++i)
        {
            float t = (float)i/(float)sampleNB;
            std::complex<float> result = cfs.apply_cfs_equation(t);
            previousPos = currentPos;
            currentPos = vec_to_sfvec2_v(cfs.world_to_screen_pos(vec_v(real(result), imag(result))));
            line[0].color = sf::Color(0, 0, 255, 255 * pointsDuration[i]);
            line[1].color = sf::Color(0, 0, 255, 255 * pointsDuration[i]);
            line[0].position = previousPos;
            line[1].position = currentPos;
            window.draw(line);
            // saveBuffer.draw(line);
            pointsDuration[i] = fmax(pointsDuration[i] - EVAPORATION_SPEED, 0);
        }
        window.draw(points);
        // saveBuffer.draw(points);
        // saveBuffer.display();
        // saveBuffer.getTexture().copyToImage().saveToFile("res/export/" + std::to_string(tick) + ".png");

        window.display();

    }

	return 0;
}