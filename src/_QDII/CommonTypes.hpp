#pragma once
#include <Cpp_Standard_Library.h>
#include <functional>
#include <Arduino.h>
namespace Quick_digital_IO_interrupt
{
	template <uint8_t Pin>
#ifdef __cpp_variable_templates
	std::move_only_function<void()> _PinIsr;
#else
	struct _PinIsr
	{
		static std::move_only_function<void()> value;
	};
	template <uint8_t Pin>
	std::move_only_function<void()> _PinIsr<Pin>::value;
#endif
	template <uint8_t Pin>
	void _CommonIsr()
	{
		_CSL_Struct14Value(_PinIsr, Pin)();
	}
	struct _PinCommonIsr
	{
		std::move_only_function<void()> &PinIsr;
		void (*CommonIsr)();
	};
	template <typename T>
	struct _PinIsrMap;
	template <typename T>
	inline std::move_only_function<void()> _Once(T &Mof)
	{
		return std::reference_wrapper<T>(Mof);
	}
	template <typename T>
	inline std::move_only_function<std::enable_if_t<!std::is_reference<T>::value>()> _Once(T &&Mof)
	{
		return std::move(Mof);
	}
}