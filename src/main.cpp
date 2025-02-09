#include <Arduino.h>
#include <U8g2lib.h>

//构造对象
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI  u8g2(U8G2_R0, 18, 13, 4, 2, 15);

// 定义按键引脚
#define LEFT_BUTTON_PIN 12
#define RIGHT_BUTTON_PIN 14

// 游戏变量
int paddleX = 60; // 板子初始位置
const int paddleWidth = 30;
const int paddleHeight = 5;
int ballX = 64; // 小球初始位置
int ballY = 32;
int ballSpeedX = 2; // 小球水平速度
int ballSpeedY = 2; // 小球垂直速度

// 消抖延迟
const int time = 10;

void setup() {
    pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
    u8g2.begin();
}

bool readButton(int pin) {
    bool state = digitalRead(pin);
    delay(time); // 消抖延迟
    return state == digitalRead(pin); // 检查状态是否稳定
}

void loop() {
    // 持续检查按键状态以实现长按移动
    if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
        paddleX = paddleX - 5; // 向左移动
        delay(100); // 控制移动速度
    }
    if (digitalRead(RIGHT_BUTTON_PIN) == LOW) {
        paddleX = paddleX + 5; // 向右移动
        delay(100); // 控制移动速度
    }

    // 限制板子位置
    if (paddleX < 0) paddleX = 0;
    if (paddleX > 128 - paddleWidth) paddleX = 128 - paddleWidth;

    // 更新小球位置
    ballX = ballX + ballSpeedX;
    ballY = ballY - ballSpeedY;

    // 碰撞检测
    if (ballX <= 0 || ballX >= 128) {
        ballSpeedX = -ballSpeedX; // 水平反弹
    }
    if (ballY <= 0) {
        ballSpeedY = -ballSpeedY; // 垂直反弹
    }
    if (ballY >= 64 - paddleHeight && ballX >= paddleX && ballX <= paddleX + paddleWidth) {
        ballSpeedY = -ballSpeedY; // 与板子碰撞
    }

    // 如果小球落到底部，重置游戏
    if (ballY > 64) {
        ballX = 64;
        ballY = 32;
        ballSpeedX = 2;
        ballSpeedY = 2;
    }

    // 清屏并绘制
    u8g2.clearBuffer();
    u8g2.drawBox(paddleX, 64 - paddleHeight, paddleWidth, paddleHeight); // 绘制板子
    u8g2.drawCircle(ballX, ballY, 3); // 绘制小球
    u8g2.sendBuffer();

    delay(20); // 控制帧率
}
