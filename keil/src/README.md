This is a small EXTI example project.
----------------------------------------
**Support External Interrupt from 0 to 15**
>
---
***Сonstructor***
* @param [in] preemption_priority - preemption priority
* @param [in] sub_priority - sub priority
* @param [in] function - the interrupt processing function
>**Exti(GPIO_TypeDef* port, uint8_t pin, Config* const config)*
---
***Set Priority Interrupt***
* @param [in] preemption_priority - preemption priority, default 0
* @param [in] sub_priority - sub priority, default 0
>**void setInterruptHandler(std::function<void()> callback, uint8_t preemption, uint8_t sub)**
---
***Enable interrupt line***
>**void enable()**
---
***Disable interrupt line***
>**void disable()**
---
***Clears the EXTI's line pending flags***
>**void clearFlag()**
---
***Checks whether the specified EXTI line flag is set or reset***
* @retval true - flag is set, false - flag is reset
>**bool isFlagStatus()**