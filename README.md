# STM32_project

This project, **STM32_project**, showcases two distinct games developed for the STM32 platform: **Brick Game** and **Chinese Chess**.

## Brick Game

The Brick Game is an interactive application where players control a paddle to bounce a ball and break bricks.
To start playing, simply touch the paddle on the screen.
Players can customize various game parameters by modifying the `brick game/Inc/user_def.h` file:

- **Ball Radius (`CIRCLE_R`)**: Defines the radius of the ball.
- **Paddle Dimensions (`HITBOARD_W`, `HITBOARD_H`)**: Set the width and height of the paddle.
- **Paddle Position (`HITBOARD_X`, `HITBOARD_Y`)**: Determine the paddle's initial coordinates on the screen.
- **Paddle Color (`HITBOARD_COLOR`)**: Specify the color of the paddle.
- **Brick Dimensions (`RECT_W`, `RECT_H`)**: Adjust the width and height of the bricks.
- **Brick Depth (`RECT_D`)**: Set the depth of the bricks.
- **Brick Gap (`RECT_G`)**: Define the gap between bricks.
- **Number of Bricks (`RECT_NUM`)**: Specify the total number of bricks in the game.
- **Game Speed (`TIMMER_SPEED`)**: Control the speed of the game.
- **Paddle Sensitivity (`SENSITIVITY`)**: Adjust the sensitivity of the paddle's movement.

For a visual reference, see the example image below:

![Brick Game Screenshot](https://github.com/space789/STM32_project/blob/main/picture/brick%20game.JPG)

## Chinese Chess

The Chinese Chess game offers a traditional two-player experience.
After touching the screen, players can engage in a match following standard Chinese Chess rules.
The game concludes with a victory screen for the winning player.

An example image of the game interface is provided below:

![Chinese Chess Screenshot](https://github.com/space789/STM32_project/blob/main/picture/Chinese%20chess.JPG)

## License

This project is licensed under the MIT License.
For more details, refer to the [LICENSE](https://github.com/space789/STM32_project/blob/main/LICENSE) file.

---
