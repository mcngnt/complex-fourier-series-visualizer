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


    float tick = 0.f;
    int sampleNB = 600;
    sf::Vector2f viewPos(SCREEN_W/2, SCREEN_H/2);
    sf::Vector2f minViewSize(SCREEN_W, SCREEN_H);
    float zoomFactor = 1.f;
    sf::View mainView(viewPos, minViewSize * zoomFactor);
    window.setView(mainView);
    std::string currentFileName = "fourier.txt";
    CFS cfs;
    float currentT;
    float pointsDuration[sampleNB] = {0};
    cfs.generate_target_eq(currentFileName);
    cfs.set_middle_point();
    cfs.calculate_coef();
    cfs.sort_coef();
    bool pressingLeftButton = false;
    sf::Vector2f oldMousePos;
    sf::Vector2f oldViewPos;
    bool isPaused = false;
    float simSpeed = 1.f;
    float tickLoop = 0.f;
    bool isSlow = false;
    bool drawOverlapCircle = true;
    // int oldTick = 0;

    while(window.isOpen())
    {
        sf::Event event;
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
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
                try
                {
                    std::string fileName;
                    std::cout << "Type name of the save (batman.txt for example) : ";
                    std::cin >> fileName;
                    cfs.generate_target_eq(fileName);
                    cfs.set_middle_point();
                    cfs.calculate_coef();
                    cfs.sort_coef();
                } catch(...)
                {
                    std::cout << "Wrong filename" << std::endl;
                }
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    isPaused = !isPaused;
                }
                if (event.key.code == sf::Keyboard::X)
                {
                    drawOverlapCircle = !drawOverlapCircle;
                }
                if (event.key.code == sf::Keyboard::E)
                {
                    isSlow = !isSlow;
                    if (isSlow)
                    {
                        simSpeed = 0.02f;
                    }
                    else
                    {
                        simSpeed = 1.f;
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                try
                {
                    std::string circNB;
                    std::cout << "Type the number of epicycloids : ";
                    std::cin >> circNB;
                    
                    cfs.circlesNB = abs(std::stoi(circNB));
                    cfs.calculate_coef();
                    cfs.sort_coef();
                }catch(...)
                {
                    std::cout << "Wrong number" << std::endl;
                }
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                zoomFactor = clamp(zoomFactor - event.mouseWheel.delta * 0.1 * zoomFactor, 0.001f, 20.f);
                mainView.setSize(minViewSize * zoomFactor);
                window.setView(mainView);
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    pressingLeftButton = true;
                    oldMousePos = mousePos;
                    oldViewPos = viewPos;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    pressingLeftButton = false;
                }
            }
        }

        // mainView.setCenter(viewPos);
        // oldTick = tick;
        if (!isPaused)
        {
            tick += simSpeed;
            tickLoop += simSpeed;
            if (tickLoop > CYCLE_DURATION)
            {
                tickLoop -= CYCLE_DURATION;
            }
        }
        if (pressingLeftButton)
        {
            viewPos = oldViewPos + (oldMousePos - mousePos) * zoomFactor;
            mainView.setCenter(viewPos);
            window.setView(mainView);
        }

       
        currentT = (float)(tickLoop)/(float)CYCLE_DURATION;
        window.clear();

        // saveBuffer.clear();
        sf::VertexArray points(sf::Points, sampleNB);
        sf::VertexArray line(sf::Lines, 2);
        sf::Vector2f currentPos = vec_to_sfvec2_v(cfs.world_to_screen_pos(vec_v(0,0)));
        sf::Vector2f previousPos = currentPos;
        sf::CircleShape circleCenter;
        circleCenter.setFillColor(sf::Color::Yellow);
        sf::CircleShape circ;
        circ.setFillColor(sf::Color::Transparent);
        sf::Color outlineCircCol = sf::Color::Green;
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
        circ.setOutlineThickness(zoomFactor);
        std::complex<float> sum(0,0);
        for (int i = 0; i < cfs.circlesNB; ++i)
        {
            float ratio = (float)i/(float)cfs.circlesNB;
            sum += cfs.apply_cfs_equation_at_n(currentT, i);
            previousPos = currentPos;
            currentPos = vec_to_sfvec2_v(cfs.world_to_screen_pos(vec_v(real(sum), imag(sum))));
            circleCenter.setRadius(SCREEN_W * 0.003 * zoomFactor * (1.f - ratio * 0.4));
            circleCenter.setOrigin(circleCenter.getRadius(), circleCenter.getRadius());
            line[0].position = previousPos;
            line[1].position = currentPos;
            circ.setPosition(previousPos);
            circ.setRadius(sqrt((previousPos.x-currentPos.x)*(previousPos.x-currentPos.x) + (previousPos.y-currentPos.y)*(previousPos.y-currentPos.y)));
            circ.setOrigin(circ.getRadius(), circ.getRadius());
            circleCenter.setPosition(currentPos);
            if (i == cfs.circlesNB - 1)
            {
                circleCenter.setFillColor(sf::Color::Magenta);
            }
            // outlineCircCol.a = (float)i/(float)cfs.circlesNB * 255.f + 50.f;
            circ.setOutlineColor(outlineCircCol);
            window.draw(line);
            window.draw(circleCenter);
            // saveBuffer.draw(line);
            if (drawOverlapCircle)
            {
                window.draw(circ);
            }
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
            if (!isPaused)
            {
                pointsDuration[i] = fmax(pointsDuration[i] - EVAPORATION_SPEED * simSpeed, 0);
            }
        }
        window.draw(points);
        // saveBuffer.draw(points);
        // saveBuffer.display();
        // saveBuffer.getTexture().copyToImage().saveToFile("res/export/" + std::to_string(tick) + ".png");

        window.display();

    }

	return 0;
}