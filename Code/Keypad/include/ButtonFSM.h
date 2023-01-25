#include <stdint.h>

// TODO rewrite as abstract class to plug in hardware setup functions per mcu

/**
 * @brief Class to manage interrupt based, software debouncing
 * FSM state and button value managed internally, outside scope calls event functions
 * function to return system time must also be passed as a pointer
 * Supports both polling and interrupt h/w strategies
 */
class ButtonFSM
{
public:
    enum States
    {
        IDLE,
        TIMING,
        ACTIVE
    };
    ButtonFSM();
    ButtonFSM(uint32_t (*)(), uint32_t);
    void init();
    void trigger();
    void fall();
    void run(bool);
    void setDebounceTime(uint32_t);
    bool getOutput();

private:
    States _st;                // Internal state of FSM
    uint32_t _timestamp;       // Marker used for timing
    uint32_t _debounceTime;    // How long the button should be continually active to be considered debounced
    uint32_t (*_getSystime)(); // Pointer for function to get systime
    bool _isActive;            // Output of the FSM
};

ButtonFSM::ButtonFSM() {}

/**
 * @brief Construct a new button F S M::button F S M object
 *
 * @param getSystime pointer to fucntion that will return system time
 * @param debounceTime how long the the pin should be active to be considered debounced
 */
ButtonFSM::ButtonFSM(uint32_t (*getSystime)(), uint32_t debounceTime)
{
    // Set initial state of all private variables
    this->_timestamp = 0;
    this->_st = IDLE;
    this->_getSystime = getSystime;
    this->_isActive = false;
    this->_debounceTime = debounceTime;
}

void ButtonFSM::init()
{
}
/***************** Transition Functions **************/
/**
 * @brief Stimulus to indicate an active rising edge
 *
 */
void ButtonFSM::trigger()
{
    if (this->_st == IDLE)
    {
        // Reset debounce timer
        this->_timestamp = this->_getSystime();
        // Move to next state
        this->_st = TIMING;
    }
}

/**
 * @brief Stimulus to indicate active falling edge
 *
 */
void ButtonFSM::fall()
{
    if (this->_st == TIMING || this->_st == ACTIVE)
    {
        // Turn off output
        this->_isActive = false;
        // Go to IDLE state and wait for a rising edge
        this->_st = IDLE;
    }
}

/**
 * @brief Change the debounce time
 *
 * @param debounceTime new debounce time
 */
void ButtonFSM::setDebounceTime(uint32_t debounceTime)
{
    this->_debounceTime = debounceTime;
}

/**
 * @brief run the state machine's internal state management. This should be calling in a loop while the button is to be polled
 *
 * @param btnActive current button state
 */
void ButtonFSM::run(bool btnActive)
{
    // Take action based on current state and button state
    switch (this->_st)
    {
    case IDLE:
        if (btnActive)
            this->trigger();
        break;
    case TIMING:
        // If the button is active and debounce timer is up, move to active state
        if (btnActive && (this->_getSystime() - this->_timestamp > this->_debounceTime))
        {
            this->_st = ACTIVE;
            this->_isActive = true;
        }
        // If the button is no longer active, trigger falling edge transition back to IDLE
        else if (!btnActive)
        {
            this->fall();
        }
        break;
    case ACTIVE:
        if (!btnActive)
            this->fall();
        break;
    }
}

bool ButtonFSM::getOutput()
{
    return this->_isActive;
}
