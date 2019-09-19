/**
 ******************************************************************************
 * @file    exti.hpp
 * @author  Ivan Orfanidi
 * @version V1.0.0
 * @date    16-08-2018
 * @brief   This file contains all the methods prototypes for External 
 *          interrupt/event controller (EXTI)
 *          firmware library.
 ******************************************************************************
 * @attention
 *
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EXTI_HPP
#define __EXTI_HPP

#include <stdint.h>

/* Driver MCU */
#include "stm32f10x.h"

/* Utils */
#include "interrupt.hpp"

#ifdef __cplusplus

/* C++ lib*/
#include <functional>

/*
 * @brief Class Exti
 */
class Exti {
  public:
    enum ExtiNum {
        EXTI_0 = 0,
        EXTI_1,
        EXTI_2,
        EXTI_3,
        EXTI_4,
        EXTI_5,
        EXTI_6,
        EXTI_7,
        EXTI_8,
        EXTI_9,
        EXTI_10,
        EXTI_11,
        EXTI_12,
        EXTI_13,
        EXTI_14,
        EXTI_15,

        EXTI_MAX_COUNT
    };

    /// GPIO Pull Up Pull Down
    enum class GpioPuPd { NOPULL, UP, DOWN };

    enum class Trigger { RISING = 0x08, FALLING = 0x0C, RISING_FALLING = 0x10 };

    enum class Mode { INTERRUPT = 0x00, EVENT = 0x04 };

    struct Config {
        GpioPuPd gpioPuPd;
        Trigger trigger;
        Mode mode;
    };

    enum Priority {
        PREEMPTION_PRIORITY = 0,
        SUB_PRIORITY = 0,
    };

    /// Ñonstructor
    Exti(GPIO_TypeDef*, uint8_t, Config* const);

    /// Deconstructor
    ~Exti();

    /// Initialisation alarm handler
    void setInterruptHandler(std::function<void()>,
        uint8_t preemption = PREEMPTION_PRIORITY,
        uint8_t sub = SUB_PRIORITY);

    void enable() const;    /// Enable interrupt line

    void disable() const;    /// Disable interrupt line

    /// Clears the EXTI's line pending flags
    void clearFlag() const;

    /// Checks whether the specified EXTI line flag is set or reset
    bool isFlagStatus() const;

    /// Interrupt Handler
    static void handler();

  private:
  
    enum PortSources {
        PORT_SOURCE_GPIOA = 0,
        PORT_SOURCE_GPIOB = 1,
        PORT_SOURCE_GPIOC = 2,
        PORT_SOURCE_GPIOD = 3,
        PORT_SOURCE_GPIOE = 4
    };

    Exti() = delete;
    Exti(const Exti&) = delete;
    Exti(Exti&&) = delete;

    Exti& operator=(const Exti&) = delete;
    Exti& operator=(Exti&&) = delete;

    void initGpio(GpioPuPd) const;    /// Configure GPIO

    void deInitGpio() const;    /// Deinitialization GPIO

    void initExti(Config* const) const;    /// Initialization EXTI Line

    void deInitExti() const;    /// Deinitialization EXTI Line

    static Exti*
        Extis[EXTI_MAX_COUNT];    ///< Main array pointers of classes Uarts

    size_t _number;

    std::function<void()> _callback;    ///< Callback handler function

    GPIO_TypeDef* _port;
    uint8_t _pin;

    size_t _channel;    ///< Interrupt channel

    PortSources _source;    ///< Port source

    uint32_t _line;

    Mode _mode;    ///< Mode (Event or Interrupt)

    Interrupt* _interrupt;    ///< Interrupt class handler
};

extern "C" {
}

#endif    //__cplusplus

#endif