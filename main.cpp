#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>

using namespace std;

void Relocate(sf::Shape&, int, int);
void SetMessage(sf::Text&, string, int);

int main(void)
{
    // Main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong 0.1");
    window.setFramerateLimit(60);

    bool paused = true;

    // Paddle 1
    sf::RectangleShape paddle1(sf::Vector2f(15, 65));
    paddle1.setFillColor(sf::Color::White);

    int paddle1PosX = 50;
    int paddle1PosY = 235;
    int paddle1Speed = 5;

    paddle1.setPosition(paddle1PosX, paddle1PosY);

    // Paddle 2
    sf::RectangleShape paddle2(sf::Vector2f(15, 65));
    paddle2.setFillColor(sf::Color::White);

    int paddle2PosX = 735;
    int paddle2PosY = 235;
    int paddle2Speed = 5;

    paddle2.setPosition(paddle2PosX, paddle2PosY);

    //Ball
    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::White);

    int ballPosX = 400;
    int ballPosY = 300;
    int ballSpeed = 5;
    int ballDirX = 1;
    int ballDirY = 1;

    ball.setPosition(ballPosX, ballPosY);

    //Game logic
    int player1Score = 0;
    int player2Score = 0;

    int winLimit = 5;
    bool playerWon = false;

    // Game text
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/TTF/comic.ttf");

    string player1Message = "Player 1: ";
    string player2Message = "Player 2: ";
    string winMessage = "CONGRATULATIONS PLAYER ";

    // Player 1 score
    sf::Text player1ScoreText(player1Message, font);
    player1ScoreText.setPosition(200, 3);
    player1ScoreText.setCharacterSize(30);
    player1ScoreText.setColor(sf::Color::White);
    SetMessage(player1ScoreText, player1Message, 0);

    // Player 2 score
    sf::Text player2ScoreText(player2Message, font);
    player2ScoreText.setPosition(460, 3);
    player2ScoreText.setCharacterSize(30);
    player2ScoreText.setColor(sf::Color::White);
    SetMessage(player2ScoreText, player2Message, 0);

    // Win message
    sf::Text winText(winMessage, font);
    winText.setPosition(35, 250);
    winText.setCharacterSize(55);
    winText.setColor(sf::Color::White);

    // Game init
    window.clear();
    window.draw(player1ScoreText);
    window.draw(player2ScoreText);
    window.draw(paddle1);
    window.draw(paddle2);
    window.draw(ball);
    window.display();

    // Main game loop
    while(window.isOpen())
    {
        sf::Event event;

       // Window handling
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        // Paddle 1 controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)
                && paddle1.getPosition().y > 0) {
            paddle1.move(0, -paddle1Speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)
                && paddle1.getPosition().y < 600 - paddle1.getSize().y) {
            paddle1.move(0, paddle1Speed);
        }

        // Paddle 2 controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)
                && paddle2.getPosition().y > 0) {
            paddle2.move(0, -paddle2Speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)
                && paddle2.getPosition().y < 600 - paddle2.getSize().y) {
            paddle2.move(0, paddle2Speed);
        }

        // Ball logic
        // Border collisions
        if(ball.getPosition().y >= 600-ball.getRadius()) ballDirY *= -1;
        if(ball.getPosition().y < 0) ballDirY *= -1;

        //Paddle collisions
        if(paddle1.getGlobalBounds().intersects(ball.getGlobalBounds())) {
            ballDirX *= -1;
            Relocate(ball, ball.getPosition().x + ballSpeed + 1, ball.getPosition().y);
        }
        if(paddle2.getGlobalBounds().intersects(ball.getGlobalBounds())) {
            ballDirX *= -1;
            Relocate(ball, ball.getPosition().x - ballSpeed + 1, ball.getPosition().y);
            
        }

        ball.move(ballSpeed*ballDirX, ballSpeed*ballDirY);

        // Game logic
        if(ball.getPosition().x >= 800-ball.getRadius() && !paused) { 
            player1Score++;
            SetMessage(player1ScoreText, player1Message, player1Score);
            paused = true;
        }
        if(ball.getPosition().x < 0 && !paused) {
            player2Score++;
            SetMessage(player2ScoreText, player2Message, player2Score);
            paused = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            paused = false;
            Relocate(ball, ballPosX, ballPosY);
            Relocate(paddle1, paddle1PosX, paddle1PosY);
            Relocate(paddle2, paddle2PosX, paddle2PosY);
            if (playerWon) {
                player1Score = 0;
                player2Score = 0;
                SetMessage(player1ScoreText, player1Message, player1Score);
                SetMessage(player2ScoreText, player2Message, player2Score);
                playerWon = false;
            }
        }

        //Win condition 
        if(player1Score == winLimit || player2Score == winLimit) {
            if (player1Score == winLimit)
            {
                SetMessage(winText, winMessage, 1);
            }

            if (player2Score == winLimit)
            {
                SetMessage(winText, winMessage, 2);
            }

            window.clear();
            window.draw(winText);
            window.display();
            paused = true;
            playerWon = true;
        }

        // Draw the game
        if (!paused) {
            window.clear();
            window.draw(player1ScoreText);
            window.draw(player2ScoreText);
            window.draw(paddle1);
            window.draw(paddle2);
            window.draw(ball);
            window.display();
        }
    }
    return 0;
}

// Relocates a Shape object to coordinates (x,y)
void Relocate(sf::Shape& ball, int x, int y) 
{
    ball.setPosition(x, y);
}

// Sets the message of a Text object to the form "msg + score"
void SetMessage(sf::Text& textObj, string msg, int score)
{
    ostringstream oss;
    oss << msg << score;
    textObj.setString(oss.str());
}
