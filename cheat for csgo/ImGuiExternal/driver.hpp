#pragma once
#include <vector>

class mem
{
public:
	std::uintptr_t pid;
	HANDLE h_proc;
public:
	mem ( int PID )
		: pid ( PID )
	{
		pid = PID;
	}

	auto init ( ) -> bool
	{
		h_proc = OpenProcess ( PROCESS_ALL_ACCESS, FALSE, pid );

		if ( pid == NULL || h_proc == NULL ) return false;
		return true;
	}

	auto write_buffer ( std::uintptr_t addr, std::uint8_t* buffer, std::size_t size ) -> bool
	{
		WriteProcessMemory ( this->h_proc, reinterpret_cast< void* >( addr ), &buffer, size, NULL );
		return true;
	}

	auto read_buffer ( std::uintptr_t addr, std::uint8_t* buffer, std::size_t size ) -> bool
	{
		ReadProcessMemory ( this->h_proc, reinterpret_cast< const void* >( addr ), &buffer, size, NULL );
		return true;
	}

	template <typename T>
	auto write ( const std::uintptr_t& address, const T& value ) -> void
	{
		WriteProcessMemory ( h_proc, reinterpret_cast< void* >( address ), &value, sizeof ( T ), NULL );
	}

	template <typename T>
	auto read ( const std::uintptr_t& address ) -> T
	{
		T value = { };
		ReadProcessMemory ( h_proc, reinterpret_cast< const void* >( address ), &value, sizeof ( T ), NULL );
		return value;
	}

	template <typename T>
	auto read_chain ( std::uintptr_t address, std::vector<std::uintptr_t> chain ) -> T
	{
		std::uintptr_t current = address;
		for ( int i = 0; i < chain.size ( ) - 1; i++ ) current = read<std::uintptr_t> ( current + chain[ i ] );

		return read<T> ( current + chain[ chain.size ( ) - 1 ] );
	}

	auto read_string ( std::uintptr_t addr ) -> std::string
	{
		char buffer[ 128 ];
		if ( read_buffer ( addr, ( std::uint8_t* )&buffer, sizeof ( buffer ) ) ) return std::string ( buffer );

		else return "None";
	}

	auto read_wstring ( std::uintptr_t addr ) -> std::wstring
	{
		wchar_t buffer[ 128 ];
		if ( read_buffer ( addr, ( std::uint8_t* )&buffer, sizeof ( buffer ) ) ) return std::wstring ( buffer, wcslen ( buffer ) );

		else return L"None";
	}
public:
	std::uintptr_t client;
	std::uintptr_t engine;
};