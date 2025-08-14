#pragma once
#include "CommonTypes.hpp"
namespace Quick_digital_IO_interrupt
{
	// 用于初始化引脚参数的内部功能，一般不应直接调用
	namespace Internal
	{
		// SAM架构不分动静态，因为其关键的variant.cpp包含中断处理函数定义，如果包含在此头文件中就会导致重复定义

#define _QDII_PinMode_Get(DS)                                                        \
	{                                                                                \
		return g_APinDescription[Pin].pPort->PIO_OSR & g_APinDescription[Pin].ulPin; \
	}
#define _QDII_PinMode_Set(DS)                                                                                                     \
	{                                                                                                                             \
		(OutOrIn ? g_APinDescription[Pin].pPort->PIO_OER : g_APinDescription[Pin].pPort->PIO_ODR) = g_APinDescription[Pin].ulPin; \
	}
#define _QDII_DigitalRead(DS)                                                                                                              \
	{                                                                                                                                      \
		return (OutOrIn ? g_APinDescription[Pin].pPort->PIO_ODSR : g_APinDescription[Pin].pPort->PIO_PDSR) & g_APinDescription[Pin].ulPin; \
	}
#define _QDII_DigitalWrite(DS)                                                                                                        \
	{                                                                                                                                 \
		(HighOrLow ? g_APinDescription[Pin].pPort->PIO_SODR : g_APinDescription[Pin].pPort->PIO_CODR) = g_APinDescription[Pin].ulPin; \
	}
#define _QDII_DigitalToggle(DS)                                                               \
	{                                                                                         \
		const PinDescription &PD = g_APinDescription[Pin];                                    \
		(PD.pPort->PIO_ODSR & PD.ulPin ? PD.pPort->PIO_CODR : PD.pPort->PIO_SODR) = PD.ulPin; \
	}
	}
	template <uint8_t... Pin>
	struct _PinIsrMap<std::integer_sequence<uint8_t, Pin...>>
	{
		static constexpr _PinCommonIsr value[NUM_DIGITAL_PINS] = {{_CSL_Struct14Value(_PinIsr, Pin), _CommonIsr<Pin>}...};
	};
	constexpr const _PinCommonIsr &_GetPinCommonIsr(uint8_t Pin)
	{
		return _PinIsrMap<std::make_integer_sequence<uint8_t, NUM_DIGITAL_PINS>>::value[Pin];
	}

	// 检查指定引脚是否已附加中断，即处于监听或挂起状态。
	inline bool InterruptEnabled(uint8_t Pin)
	{
		return g_APinDescription[Pin].ulPin & g_APinDescription[Pin].pPort->PIO_IMR;
	}
	// 检查指定引脚是否已附加中断，即处于监听或挂起状态。
	template <uint8_t Pin>
	inline bool InterruptEnabled()
	{
		return g_APinDescription[Pin].ulPin & g_APinDescription[Pin].pPort->PIO_IMR;
	}

	// 停止处理指定引脚的中断。如果引脚空闲或空闲但挂起，也不会产生异常。此方法不会析构AttachInterrupt传入的可调用对象。此方法是AttachInterrupt的逆操作。
	inline void DetachInterrupt(uint8_t Pin)
	{
		g_APinDescription[Pin].pPort->PIO_IDR = g_APinDescription[Pin].ulPin;
	}
	// 停止处理指定引脚的中断。如果引脚空闲或空闲但挂起，也不会产生异常。此方法不会析构AttachInterrupt传入的可调用对象。此方法是AttachInterrupt的逆操作。
	template <uint8_t Pin>
	inline void DetachInterrupt()
	{
		g_APinDescription[Pin].pPort->PIO_IDR = g_APinDescription[Pin].ulPin;
	}

	// 检查引脚是否有中断事件挂起：若有，则取消挂起。由于SAM架构设计使然，此方法还会同时清除指定引脚之外的某些引脚的挂起状态，详见SAM架构数据表。但是，此方法确保只返回指定引脚的挂起状态。
	template <uint8_t Pin>
	inline bool ClearInterruptPending()
	{
		return g_APinDescription[Pin].pPort->PIO_ISR & g_APinDescription[Pin].ulPin;
	}
	// 检查引脚是否有中断事件挂起：若有，则取消挂起。由于SAM架构设计使然，此方法还会同时清除指定引脚之外的某些引脚的挂起状态，详见SAM架构数据表。但是，此方法确保只返回指定引脚的挂起状态。
	inline bool ClearInterruptPending(uint8_t Pin)
	{
		return g_APinDescription[Pin].pPort->PIO_ISR & g_APinDescription[Pin].ulPin;
	}

	// 检查全局设置中断是否启用。如禁用，所有引脚的中断均不工作。使用内置interrupts()和noInterrupts()来启用和禁用全局中断。
	inline bool GlobalInterruptEnabled()
	{
		return !__get_PRIMASK();
	}
}