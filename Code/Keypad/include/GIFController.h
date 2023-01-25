
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

class GIFController
{
private:
    uint8_t _frame_count; // Number of frames in the GIF
    uint8_t _frame_index;
    uint32_t _frame_time;
    Adafruit_SSD1306 _display;
    uint8_t *_img; // Pointer to GIF bitmap

public:
    GIFController(uint8_t *img, uint8_t frame_count, TwoWire *twi, uint32_t frame_period);
    bool init();
    void nextFrame();
    void changeGIF(uint8_t *newImg, uint8_t frame_count);
    void loop(uint32_t);
    void setFrameTime(uint32_t);
};

/**
 * @brief Construct a new GIFController::GIFController object
 *
 * @param img Pointer to gif bitmap
 * @param frame_count # of frames in curreng gif
 * @param twi pointer to 2-wire interface object
 * @param frame_period Period of time in ms that each frame lasts
 */
GIFController::GIFController(uint8_t *img, uint8_t frame_count, TwoWire *twi, uint32_t frame_period)
{
    this->_img = img;
    this->_frame_count = frame_count;
    this->_display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, twi, OLED_RESET);
    this->_frame_index = 0;
    this->_frame_time = frame_period;
}

bool GIFController::init()
{
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    return this->_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void GIFController::nextFrame()
{
    this->_display.clearDisplay();
    this->_display.drawBitmap(0, 0, this->_img + _frame_index * 1024, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
    this->_frame_index = (this->_frame_index + 1) % this->_frame_count;
    this->_display.display();
}

void GIFController::changeGIF(uint8_t *newImg, uint8_t frameCount)
{
    this->_img = newImg;
    this->_frame_count = frameCount;
    this->_frame_index = 0;
}

/**
 * @brief Update the period for each frame
 * 
 * @param newTime 
 */
void GIFController::setFrameTime(uint32_t newTime)
{
    this->_frame_time = newTime;
}

void GIFController::loop(uint32_t sysTime)
{
    static uint32_t timer = 0;
    if (sysTime - timer > this->_frame_time)
    {
        this->nextFrame();
        timer = sysTime;
    }
}