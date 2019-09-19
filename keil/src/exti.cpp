/**
 ******************************************************************************
 * @file    exit.cpp
 * @author  Ivan Orfanidi
 * @version V1.0.0
 * @date    28-March-2018
 * @brief   This file provides all the External Interrupt firmware method.
 ******************************************************************************
 * @attention
 *
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "exti.hpp"

/*
 * Main array pointers of classes Extis
 * Use EXTI_MAX_COUNT max quantity Extis
 */
Exti* Exti::Extis[EXTI_MAX_COUNT];

/**
 * @brief Ñonstructor
 * @param [in] preemption_priority - preemption priority
 * @param [in] sub_priority - sub priority
 * @param [in] function - the interrupt processing function
 */
Exti::Exti(GPIO_TypeDef* port, uint8_t pin, Config* const config) :
    _port(port),
    _pin(pin),
    _mode(config->mode),
    _line(1 << pin),
    _interrupt(nullptr)
{
    // Enable GPIO clock
    if(_port == GPIOA) {
        _source = PORT_SOURCE_GPIOA;
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
    }
    else if(_port == GPIOB) {
        _source = PORT_SOURCE_GPIOB;
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
    }
    else if(_port == GPIOC) {
        _source = PORT_SOURCE_GPIOC;
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;
    }
    else if(_port == GPIOD) {
        _source = PORT_SOURCE_GPIOD;
        RCC->APB2ENR |= RCC_APB2ENR_IOPDEN | RCC_APB2ENR_AFIOEN;
    }
    else if(_port == GPIOE) {
        _source = PORT_SOURCE_GPIOE;
        RCC->APB2ENR |= RCC_APB2ENR_IOPEEN | RCC_APB2ENR_AFIOEN;
    }

    switch(_pin) {
        case 0:
            _channel = EXTI0_IRQn;
            _number = EXTI_0;
            break;
        case 1:
            _channel = EXTI1_IRQn;
            _number = EXTI_1;
            break;
        case 2:
            _channel = EXTI2_IRQn;
            _number = EXTI_2;
            break;
        case 3:
            _channel = EXTI3_IRQn;
            _number = EXTI_3;
            break;
        case 4:
            _channel = EXTI4_IRQn;
            _number = EXTI_4;
            break;
        case 5:
            _channel = EXTI9_5_IRQn;
            _number = EXTI_5;
            break;
        case 6:
            _channel = EXTI9_5_IRQn;
            _number = EXTI_6;
            break;
        case 7:
            _channel = EXTI9_5_IRQn;
            _number = EXTI_7;
            break;
        case 8:
            _channel = EXTI9_5_IRQn;
            _number = EXTI_8;
            break;
        case 9:
            _channel = EXTI9_5_IRQn;
            _number = EXTI_9;
            break;
        case 10:
            _channel = EXTI15_10_IRQn;
            _number = EXTI_10;
            break;
        case 11:
            _channel = EXTI15_10_IRQn;
            _number = EXTI_11;
            break;
        case 12:
            _channel = EXTI15_10_IRQn;
            _number = EXTI_12;
            break;
        case 13:
            _channel = EXTI15_10_IRQn;
            _number = EXTI_13;
            break;
        case 14:
            _channel = EXTI15_10_IRQn;
            _number = EXTI_14;
            break;

        default:
            _channel = EXTI15_10_IRQn;
            _number = EXTI_15;
    };

    if(Extis[_number] != nullptr) {
        return;
    }

    // Configure pin as input floating
    initGpio(config->gpioPuPd);

    // Configure EXTI line
    initExti(config);

    // Disable default
    disable();

    Extis[_number] = this;
}

/**
 * @brief Deconstructor class Exti
 */
Exti::~Exti()
{
    disable();

    // Disable Interrupt
    if(_interrupt != nullptr) {
        ::delete _interrupt;
    }

    deInitExti();

    deInitGpio();

    Extis[_number] = nullptr;
}

/**
 * @brief Initialization GPIO for EXTI
 */
void Exti::initGpio(GpioPuPd gpioPuPd) const
{
    switch(_pin) {
        case 0:
            _port->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
            _port->CRL |= GPIO_CRL_CNF0_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS0;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR0;
            }
            break;
        case 1:
            _port->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
            _port->CRL |= GPIO_CRL_CNF1_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS1;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR1;
            }
            break;
        case 2:
            _port->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
            _port->CRL |= GPIO_CRL_CNF2_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS2;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR2;
            }
            break;
        case 3:
            _port->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
            _port->CRL |= GPIO_CRL_CNF3_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS3;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR3;
            }
            break;
        case 4:
            _port->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
            _port->CRL |= GPIO_CRL_CNF4_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS4;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR4;
            }
        case 5:
            _port->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
            _port->CRL |= GPIO_CRL_CNF5_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS5;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR5;
            }
            break;
        case 6:
            _port->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
            _port->CRL |= GPIO_CRL_CNF6_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS6;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR6;
            }
            break;
        case 7:
            _port->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
            _port->CRL |= GPIO_CRL_CNF7_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS7;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR7;
            }
            break;
        case 8:
            _port->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_MODE8);
            _port->CRH |= GPIO_CRH_CNF8_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS8;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR8;
            }
            break;
        case 9:
            _port->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
            _port->CRH |= GPIO_CRH_CNF9_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS9;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR9;
            }
            break;
        case 10:
            _port->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
            _port->CRH |= GPIO_CRH_CNF10_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS10;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR10;
            }
            break;
        case 11:
            _port->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_MODE11);
            _port->CRH |= GPIO_CRH_CNF11_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS11;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR11;
            }
            break;
        case 12:
            _port->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_MODE12);
            _port->CRH |= GPIO_CRH_CNF12_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS12;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR12;
            }
            break;
        case 13:
            _port->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
            _port->CRH |= GPIO_CRH_CNF13_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS13;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR13;
            }
            break;
        case 14:
            _port->CRH &= ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14);
            _port->CRH |= GPIO_CRH_CNF14_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS14;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR14;
            }
            break;
        case 15:
            _port->CRH &= ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
            _port->CRH |= GPIO_CRH_CNF15_1;

            if(gpioPuPd == GpioPuPd::UP) {
                // Pull-Up
                _port->BSRR |= GPIO_BSRR_BS15;
            }
            else if(gpioPuPd == GpioPuPd::DOWN) {
                // Pull-Down
                _port->BSRR |= GPIO_BSRR_BR15;
            }
            break;

        default:
            break;
    }

    const uint32_t temp = 0x0F << (0x04 * (_pin & 0x03));
    AFIO->EXTICR[_pin >> 0x02] &= !temp;
    AFIO->EXTICR[_pin >> 0x02] |= (_source << (0x04 * (_pin & 0x03)));
}

/**
 * @brief  Deinitialization GPIO for Exti.
 */
void Exti::deInitGpio() const
{
    switch(_pin) {
        case 0:
            _port->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
            _port->BSRR &= ~GPIO_BSRR_BS0;
            _port->BSRR &= ~GPIO_BSRR_BR0;
            break;
        case 1:
            _port->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
            _port->BSRR &= ~GPIO_BSRR_BS1;
            _port->BSRR &= ~GPIO_BSRR_BR1;
            break;
        case 2:
            _port->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
            _port->BSRR &= ~GPIO_BSRR_BS2;
            _port->BSRR &= ~GPIO_BSRR_BR2;
            break;
        case 3:
            _port->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
            _port->BSRR &= ~GPIO_BSRR_BS3;
            _port->BSRR &= ~GPIO_BSRR_BR3;
            break;
        case 4:
            _port->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
            _port->BSRR &= ~GPIO_BSRR_BS4;
            _port->BSRR &= ~GPIO_BSRR_BR4;
        case 5:
            _port->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
            _port->BSRR &= ~GPIO_BSRR_BS5;
            _port->BSRR &= ~GPIO_BSRR_BR5;
            break;
        case 6:
            _port->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
            _port->BSRR &= ~GPIO_BSRR_BS6;
            _port->BSRR &= ~GPIO_BSRR_BR6;
            break;
        case 7:
            _port->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
            _port->BSRR &= ~GPIO_BSRR_BS7;
            _port->BSRR &= ~GPIO_BSRR_BR7;
            break;
        case 8:
            _port->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_MODE8);
            _port->BSRR &= ~GPIO_BSRR_BS8;
            _port->BSRR &= ~GPIO_BSRR_BR8;
            break;
        case 9:
            _port->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
            _port->BSRR &= ~GPIO_BSRR_BS9;
            _port->BSRR &= !GPIO_BSRR_BR9;
            break;
        case 10:
            _port->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
            _port->BSRR &= ~GPIO_BSRR_BS10;
            _port->BSRR &= ~GPIO_BSRR_BR10;
            break;
        case 11:
            _port->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_MODE11);
            _port->BSRR &= ~GPIO_BSRR_BS11;
            _port->BSRR &= ~GPIO_BSRR_BR11;
            break;
        case 12:
            _port->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_MODE12);
            _port->BSRR &= ~GPIO_BSRR_BS12;
            _port->BSRR &= ~GPIO_BSRR_BR12;
            break;
        case 13:
            _port->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
            _port->BSRR &= ~GPIO_BSRR_BS13;
            _port->BSRR &= ~GPIO_BSRR_BR13;
            break;
        case 14:
            _port->CRH &= ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14);
            _port->BSRR &= ~GPIO_BSRR_BS14;
            _port->BSRR &= ~GPIO_BSRR_BR14;
            break;
        case 15:
            _port->CRH &= ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
            _port->BSRR &= ~GPIO_BSRR_BS15;
            _port->BSRR &= ~GPIO_BSRR_BR15;
        default:
            break;
    }

    const uint32_t temp = 0x0F << (0x04 * (_pin & 0x03));
    AFIO->EXTICR[_pin >> 0x02] &= !temp;
    AFIO->EXTICR[_pin >> 0x02] &= ~(_source << (0x04 * (_pin & 0x03)));
}

/**
 * @brief Initialization EXTI Lines EXTI Line
 */
void Exti::initExti(Config* const config) const
{
    deInitExti();

    // Trigger
    if(config->trigger == Trigger::FALLING ||
        config->trigger == Trigger::RISING_FALLING) {
        EXTI->FTSR |= _line;
    }

    if(config->trigger == Trigger::RISING ||
        config->trigger == Trigger::RISING_FALLING) {
        EXTI->RTSR |= _line;
    }
}

/**
 * @brief Deinitialization EXTI Line
 */
void Exti::deInitExti() const
{
    EXTI->IMR &= _line;
    EXTI->EMR &= _line;
}

/**
 * @bref Set Priority Interrupt
 * @param [in] preemption_priority - preemption priority, default 0
 * @param [in] sub_priority - sub priority, default 0
 */
void Exti::setInterruptHandler(std::function<void()> callback,
    uint8_t preemption,
    uint8_t sub)
{
    switch(_pin) {
        case 0:
            _channel = EXTI0_IRQn;
            break;

        case 1:
            _channel = EXTI1_IRQn;
            break;

        case 2:
            _channel = EXTI2_IRQn;
            break;

        case 3:
            _channel = EXTI3_IRQn;
            break;

        case 4:
            _channel = EXTI4_IRQn;
            break;

        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            _channel = EXTI9_5_IRQn;
            break;

        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            _channel = EXTI15_10_IRQn;
            break;

        default:
            break;
    };

    _callback = callback;
    // Enable interrupt
    if(_interrupt != nullptr) {
        ::delete _interrupt;
    }
    _interrupt = ::new Interrupt(_channel, handler, preemption, sub);
}

/**
 * @brief Enable interrupt line
 */
void Exti::enable() const
{
    if(_mode == Mode::INTERRUPT) {
        // Enable interrupts
        EXTI->IMR |= _line;
    }
    else {
        // Enable events
        EXTI->EMR |= _line;
    }
}

/**
 * @brief Disable interrupt line
 */
void Exti::disable() const
{
    if(_mode == Mode::INTERRUPT) {
        // Enable interrupts
        EXTI->IMR &= ~_line;
    }
    else {
        // Enable events
        EXTI->EMR &= ~_line;
    }
}

/**
 * @bref Clears the EXTI's line pending flags
 */
void Exti::clearFlag() const
{
    EXTI->PR = _line;
}

/**
 * @bref Checks whether the specified EXTI line flag is set or reset
 * @retval true - flag is set, false - flag is reset
 */
bool Exti::isFlagStatus() const
{
    return (EXTI->PR & _line);
}

/**
 * @bref Global and static interrupt handler
 */
void Exti::handler()
{
    for(size_t i = 0; i < EXTI_MAX_COUNT; ++i) {
        if(Extis[i] != nullptr && Extis[i]->isFlagStatus()) {
            Extis[i]->_callback();
            Extis[i]->clearFlag();
        }
    }
}
