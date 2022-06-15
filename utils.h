namespace utils
{
    auto get_system_information( SYSTEM_INFORMATION_CLASS information_class ) ->  void *
    {
        unsigned long size = 32;
        char buffer[32];

        ZwQuerySystemInformation( information_class, buffer, size, &size );

        void *info = ExAllocatePoolZero( NonPagedPool, size, 7265746172 );

        if ( !info )
            return nullptr;

        if ( !NT_SUCCESS( ZwQuerySystemInformation( information_class, info, size, &size ) ) )
        {
            ExFreePool( info );
            return nullptr;
        }

        return info;
    }

    typedef struct _image_t
    {
        uintptr_t base;
        size_t size;
    }image_t, *pimage_t;

    auto get_kernel_module( const char *name ) -> image_t
    {
        const PRTL_PROCESS_MODULES info = ( PRTL_PROCESS_MODULES )get_system_information( SystemModuleInformation );

        if ( !info )
        {
            return { 0, 0 };
        }

        for ( size_t i = 0; i < info->NumberOfModules; ++i )
        {
            const auto &mod = info->Modules[i];

            if ( crt::strcmp( to_lower( ( char * )mod.FullPathName + mod.OffsetToFileName ), name ) == 0 )
            {
                _image_t image = { };
                image.base = (uintptr_t)mod.ImageBase;
                image.size = ( size_t )mod.ImageBase;
                ExFreePool( info );
                return image;
            }
        }

        ExFreePool( info );
        return { 0, 0 };
    }
}