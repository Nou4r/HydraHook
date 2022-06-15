namespace eac
{
	typedef __int64( __fastcall *ex_allocate_pool_with_tag_t )( INT64, size_t, uintptr_t );

	ex_allocate_pool_with_tag_t ex_allocate_pool_with_tag_fn = nullptr;
	auto hk_allocation( INT64 type, size_t size, uintptr_t tag ) -> __int64 __fastcall
	{
		if ( size == 33096 + 16 )
		{
			DbgPrintEx( 0, 0, "EasyAntiCheat Hydra packet denied\n" );
			return 0;
		}

		return ex_allocate_pool_with_tag_fn( type, size, tag );
	}

	auto hook_allocation( const uintptr_t base, const size_t size ) -> bool
	{
		if ( size <= 0x1000 )
		{
			return false;
		}

		const auto *dos = reinterpret_cast< IMAGE_DOS_HEADER * >( base );

		if ( dos->e_magic != IMAGE_DOS_SIGNATURE )
		{
			return false;
		}

		const auto *nt = reinterpret_cast< IMAGE_NT_HEADERS * >( base + dos->e_lfanew );

		if ( nt->Signature != IMAGE_NT_SIGNATURE )
		{
			return false;
		}

		ex_allocate_pool_with_tag_fn = *( ex_allocate_pool_with_tag_t * )( base + 0xD7B80 );
		*reinterpret_cast<uintptr_t*>( base + 0xD7B80 ) = reinterpret_cast<uintptr_t>( &hk_allocation );

		return true;
	}
}