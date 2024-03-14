#include <SFML/Graphics.hpp>

int main()
{
    // Глобальный скейл для всех спрайтов в игре. Т.к. изначально в текстуре они слишком маленькие
    const sf::Vector2f drawScale(3.f, 3.f);
    // Гравитация в игре. То с какой скоростью объекты будут падать вниз
    const float gravity = -2.f;
    
    // Создание основного игрового окна. С размером 800*600
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mario");

    // Расположение земли по координате Y. Высота земли = 200. Располагается внизу окна, поэтому для расчета положенеия земли мы используем размер окна
    const float groundTop = window.getSize().y - 200.f;
    
    // Загружаем текстуру земли из отдельного файла
    sf::Texture groundTexture;
    groundTexture.loadFromFile("Resources/Ground.png");
    // Создаем спрайт для отрисовки земли в нужном положении
    sf::Sprite groundSprite;
    groundSprite.setTexture(groundTexture);
    groundSprite.setPosition(0.f, groundTop);
    
    // Текстура с тайлсетом
    sf::Texture tileSetTexture;
    // Загрузка текстуры из файла
    tileSetTexture.loadFromFile("Resources/TileSet.png");

    const sf::Vector2f playerSize(16.f, 16.f);
    // Спрайт игрока. Спрайт это часть текстуры с указанием координат в текстуре
    // Спрайт не принимает участие в расчете игровой логики. Он нужен только для отрисовки
    sf::Sprite playerSprite;
    // Загружаем текстуру в спрайт
    playerSprite.setTexture(tileSetTexture);
    // Указываем какую часть текстуры будет использовать спрайт
    playerSprite.setTextureRect(sf::IntRect(117, 507, playerSize.x, playerSize.y));
    // Задаём скейл спрайту чтобы его увеличить
    playerSprite.setScale(drawScale);
    
    // Создание переменной для хранения размера и положения игрока
    sf::FloatRect playerRect(100.f, 400.f, playerSize.x * drawScale.x, playerSize.y * drawScale.y);

    // Текущее ускорение игрока. Показывает в какую сторону и на сколько быстро двигается игрок
    sf::Vector2f playerVelocity(0.f, 0.f);
    // Скорость игрока. Безразмерная величина. Указывает как быстро может двигаться игрок
    float playerSpeed = 220.f;

    // Скорость прыжка игрока аналогично playerSpeed
    float playerJumpSpeed = 500.f;
    // Переменная которая указывает находится ли игрок на земле. Это нужно для опеределия возможности прыжка
    bool bIsPlayerOnGround = false;

    // Используется для подсчета времени обработки игрового цикла
    sf::Clock clock;
    
    // Основной цикл игры. Происходит пока окно не закрыто
    while (window.isOpen())
    {
        // Остановка выполнения программы на какое-то время. Чтобы детерминировать время обработки игрового цикла, т.к. оно очень маленькое
        sf::sleep(sf::seconds(0.001f));
        
        // Получаем время которое прошло с момента предыдущего обновления игрового цикла
        float clockDeltaSeconds = clock.getElapsedTime().asSeconds();
        // Сбрасываем счетчик времени. Чтобы в начале следующего цикла получить новое актуальное время
        clock.restart();
        
        // Опрос окна о событиях
        sf::Event event;
        // Опрашиваем все события которые произошли в окне с момента его последней отрисовки
        while (window.pollEvent(event))
        {
            // Если было вызвано событие Закрыть, то закрываем окно
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Space)
                {
                    // Если нажали кнопку прыжка и игрок на земле, то запускаем логику прыжка
                    if(bIsPlayerOnGround)
                    {
                        bIsPlayerOnGround = false;
                        playerVelocity.y = playerJumpSpeed * clockDeltaSeconds;
                    }
                }
            }
        }
        // Обработка инпута с кнопок
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            playerVelocity.x = -playerSpeed * clockDeltaSeconds;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            playerVelocity.x = playerSpeed * clockDeltaSeconds;
        }

        // Основной блок обработки игровой логики
        {
            // На игрока постоянно оказывает влияние гравитация и тянет его вниз
            // Во время прыжка она будет всегда замедлять прыжок пока игрок не начнет двигаться вниз
            playerVelocity.y += gravity * clockDeltaSeconds;

            playerRect.left += playerVelocity.x;
            playerRect.top -= playerVelocity.y;

            // Определяем приземлился ли игрок на землю
            // Для этого сравниваем его нижнюю координату с землей
            // Если нижняя координата ниже земли, то считаем что игрок на земле и не даём ему упасть ниже
            const float playerRectBottom = playerRect.top + playerRect.height; 
            if(playerRectBottom > groundTop)
            {
                bIsPlayerOnGround = true;
                playerVelocity.y = 0.f;
                playerRect.top = groundTop - playerRect.height;
            }
            

            playerVelocity.x = 0.f;
        }

        // Блок отрисовки окна
        {
            // Очищаем окно от предыдущих изображений
            window.clear();
            // Отрисовываем новые данные в окне,
            window.draw(groundSprite);
            
            playerSprite.setPosition(playerRect.left, playerRect.top);
            window.draw(playerSprite);
            // Выводим отрисованное на экран
            window.display();    
        }
    }

    return 0;
}