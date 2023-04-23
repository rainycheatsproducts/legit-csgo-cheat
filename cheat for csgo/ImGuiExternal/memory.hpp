#pragma once
#include "imports.hpp"
#include "driver.hpp"

namespace utils
{
	auto get_process_id ( LPCSTR ProcessName ) -> DWORD
	{
		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 );
		pt.dwSize = sizeof ( PROCESSENTRY32 );
		if ( Process32First ( hsnap, &pt ) ) {
			do {
				if ( !strcmp ( pt.szExeFile, ProcessName ) ) {
					CloseHandle ( hsnap );
					return pt.th32ProcessID;
				}
			} while ( Process32Next ( hsnap, &pt ) );
		}
		CloseHandle ( hsnap );
		return 0;
	}

	auto get_module ( std::int32_t pid, const wchar_t* name ) -> std::uintptr_t
	{
		const auto handle = OpenProcess ( PROCESS_QUERY_INFORMATION, 0, pid );

		auto current = 0ull;
		auto mbi = MEMORY_BASIC_INFORMATION ( );

		while ( VirtualQueryEx ( handle, reinterpret_cast< void* >( current ), &mbi, sizeof ( MEMORY_BASIC_INFORMATION ) ) )
		{
			if ( mbi.Type == MEM_MAPPED || mbi.Type == MEM_IMAGE )
			{
				const auto buffer = malloc ( 1024 );
				auto bytes = std::size_t ( );

				const static auto ntdll = GetModuleHandle ( "ntdll" );
				const static auto nt_query_virtual_memory_fn = reinterpret_cast<
					NTSTATUS ( __stdcall* )( HANDLE, void*, std::int32_t, void*, std::size_t, std::size_t* ) > (
						GetProcAddress ( ntdll, "NtQueryVirtualMemory" ) );

				if ( nt_query_virtual_memory_fn ( handle, mbi.BaseAddress, 2, buffer, 1024, &bytes ) != 0 ||
					!wcsstr ( static_cast< UNICODE_STRING* >( buffer )->Buffer, name ) ||
					wcsstr ( static_cast< UNICODE_STRING* >( buffer )->Buffer, L".mui" ) )
				{
					free ( buffer );
					goto skip;
				}

				free ( buffer );
				CloseHandle ( handle );

				return reinterpret_cast< std::uintptr_t >( mbi.BaseAddress );
			}

		skip:
			current = reinterpret_cast< std::uintptr_t >( mbi.BaseAddress ) + mbi.RegionSize;
		}

		CloseHandle ( handle );
		return 0ull;
	}

	auto find_cave ( std::int32_t pid, std::int32_t min_address = 0, std::int32_t start_size = 0 ) -> std::pair<std::uintptr_t, std::size_t>
	{
		const auto handle = OpenProcess ( PROCESS_QUERY_LIMITED_INFORMATION, 0, pid );
		auto code_cave = std::pair<std::uintptr_t, std::size_t>{ 0ull, start_size };

		auto mbi = MEMORY_BASIC_INFORMATION ( );
		auto current = 0ull;

		while ( VirtualQueryEx ( handle, ( void* )current, &mbi, sizeof ( mbi ) ) )
		{
			current = reinterpret_cast< uintptr_t >( mbi.BaseAddress ) + mbi.RegionSize;
			if ( mbi.AllocationProtect == PAGE_EXECUTE_READWRITE
				&& mbi.State == MEM_COMMIT
				&& mbi.Type == MEM_PRIVATE
				&& reinterpret_cast< uintptr_t >( mbi.BaseAddress ) > min_address )
			{
				if ( std::get<1> ( code_cave ) < mbi.RegionSize )
					code_cave = { reinterpret_cast< std::uintptr_t >( mbi.BaseAddress ), mbi.RegionSize };
			}
		}

		CloseHandle ( handle );
		return code_cave;
	}

	auto random_string ( int len ) -> std::string
	{
		srand ( time ( NULL ) );
		std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		std::string newstr;
		int pos;
		while ( newstr.size ( ) != len )
		{
			pos = ( ( rand ( ) % ( str.size ( ) - 1 ) ) );
			newstr += str.substr ( pos, 1 );
		}
		return newstr;
	}

	namespace globals
	{
		auto process_id = utils::get_process_id ( "csgo.exe" );
		auto h_proc = OpenProcess ( PROCESS_ALL_ACCESS, FALSE, process_id );
	}
}