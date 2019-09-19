/**
 ******************************************************************************
 * @file    uart.hpp
 * @author  Ivan Orfanidi
 * @version V1.0.0
 * @date    04-2019
 * @brief   This is file realise usart.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INTERRUPT_HPP
#define __INTERRUPT_HPP

/* Includes ------------------------------------------------------------------*/
/* Standart lib */
#include <stdint.h>

/* Driver MCU */
#include "stm32f10x.h"

#ifdef __cplusplus

/* C++ lib*/
#include <functional>

/*
 * @brief Class Interrupt
 */
class Interrupt {
  public:
    enum Default {
        PREEMPTION_PRIORITY = 0,
        SUB_PRIORITY = 0,
    };

    enum NumberOfInterrupts : size_t {
#ifdef STM32F10X_LD
        NVIC_MAX_COUNT = USBWakeUp_IRQn
#endif
#ifdef STM32F10X_LD_VL
            NVIC_MAX_COUNT = TIM7_IRQn
#endif
#ifdef STM32F10X_MD
                NVIC_MAX_COUNT = USBWakeUp_IRQn
#endif
#ifdef STM32F10X_MD_VL
                    NVIC_MAX_COUNT = TIM7_IRQn
#endif
#ifdef STM32F10X_HD
                        NVIC_MAX_COUNT = DMA2_Channel4_5_IRQn
#endif
#ifdef STM32F10X_HD_VL
                            NVIC_MAX_COUNT = DMA2_Channel5_IRQn
#endif
#ifdef STM32F10X_XL
                                NVIC_MAX_COUNT = DMA2_Channel4_5_IRQn
#endif
#ifdef STM32F10X_CL
                                    NVIC_MAX_COUNT = OTG_FS_IRQn
#endif
    };

    /**
    @code  
     The table below gives the allowed values of the pre-emption priority and subpriority according
     to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
      ==========================================================================
      Priority Group   | Preemption Priority | Sub Priority  | Description
      ==========================================================================
       _0              |         0           |      0-15     |   0 bits for pre-emption priority
                       |                     |               |   4 bits for subpriority
      --------------------------------------------------------------------------
       _1              |         0-1         |      0-7      |   1 bits for pre-emption priority
                       |                     |               |   3 bits for subpriority
      --------------------------------------------------------------------------   
       _2              |        0-3          |       0-3     |   2 bits for pre-emption priority
                       |                     |               |   2 bits for subpriority
      --------------------------------------------------------------------------    
       _3              |        0-7          |       0-1     |   3 bits for pre-emption priority
                       |                     |               |   1 bits for subpriority
      --------------------------------------------------------------------------    
       _4              |       0-15          |       0       |   4 bits for pre-emption priority
                       |                     |               |   0 bits for subpriority                       
      ==========================================================================
    @endcode
    */

    /// Preemption Priority Group
    enum class PriorityGroup : uint32_t {
        _0 = 0x0700,
        _1 = 0x0600,
        _2 = 0x0500,
        _3 = 0x0400,
        _4 = 0x0300
    };

    /// Distructor
    Interrupt(size_t,
        std::function<void()>,
        uint32_t preemption = PREEMPTION_PRIORITY,
        uint32_t sub = SUB_PRIORITY);

    /// Distructor (disable interrupt)
    ~Interrupt();

    /// Interrupt handler
    static void handler(size_t);

    /// Sets the vector table location and Offset.
    static void setVectorTable(uint32_t, uint32_t);

    /// Configures the priority grouping
    static void setPriorityGroup(PriorityGroup);

    void enable() const;

    void disable() const;

  private:
    Interrupt() = delete;                    /// Constructor default is delete
    Interrupt(const Interrupt&) = delete;    /// Constructor copy is delete
    Interrupt(Interrupt&&) = delete;         /// Constructor move is delete

    Interrupt& operator=(
        const Interrupt&) = delete;                /// Operator copy is delete
    Interrupt& operator=(Interrupt&&) = delete;    /// Operator move is delete

    size_t _channel;    ///< Interrupt channel

    /// Interrupt handler callback
    static std::function<void()> _callback[NVIC_MAX_COUNT];
};

extern "C" {
/* ALL INTERRUPT */
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void RTC_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
};

#endif    //__cplusplus

#endif