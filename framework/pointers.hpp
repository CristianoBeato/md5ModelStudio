
#ifndef __AUTO_POINTER_HPP__
#define __AUTO_POINTER_HPP__

#include <SDL3/SDL_atomic.h>

template<typename _t>
class crAutoPointer
{
public:
    typedef _t&         reference;
    typedef _t*         pointer;
    typedef const _t&   const_reference;
    typedef const _t*   const_pointer;

    crAutoPointer( void ) : m_pointer( nullptr )
    {
    }

    crAutoPointer( const crAutoPointer& r )
    {
        DecRef( m_pointer );
        m_pointer = r.m_pointer;
        IncRef( m_pointer );
    }

    ~crAutoPointer( void )
    {
        /// Decrement the references, if are the last, delete it
        if( !DecRef( m_pointer ) )
            DeleteThis();
    }

    template <typename... Args> 
    static inline crAutoPointer<_t> New( Args&&... in_args )
    {
        crAutoPointer<_t> newPointer;

        // Try alloc a pointer
        void* ptr = SDL_malloc( sizeof(_t) + sizeof( SDL_AtomicU32 ) );
        if (!ptr) 
            throw std::bad_alloc();

        // Set counter to one
        *reinterpret_cast<uint32_t*>( ptr ) = 1;
        
        // hide the counter 
        newPointer.m_pointer = reinterpret_cast<pointer>( static_cast<unsigned char*>( ptr ) + sizeof( SDL_AtomicU32 ) );

        newPointer.m_pointer = new ( newPointer.m_pointer ) _t( std::forward<Args>(in_args)...);

        return newPointer;
    }

    static inline crAutoPointer<_t> Alloc( const uint32_t in_count )
    {
        crAutoPointer<_t> newPointer;

        // Try alloc a pointer
        void* ptr = SDL_malloc( ( sizeof(_t) * in_count ) + sizeof( SDL_AtomicU32 ) );
        if (!ptr) 
            throw std::bad_alloc();

        // Set counter to one
        *reinterpret_cast<uint32_t*>( ptr ) = 1;
        
        // hide the counter 
        newPointer.m_pointer = reinterpret_cast<pointer>( static_cast<unsigned char*>( ptr ) + sizeof( SDL_AtomicU32 ) );

        return newPointer;
    }

    crAutoPointer& operator = ( const crAutoPointer& r )
    {
        auto old = m_pointer;
        m_pointer = r.m_pointer;
        IncRef( m_pointer );
        DecRef( old );
        return *this;
    }

    inline pointer operator -> ( void )
    {
        return m_pointer;
    }

    inline const_pointer operator -> ( void ) const
    {
        return m_pointer;
    }

    inline reference operator *( void ) 
    {
        return *m_pointer;
    }

    inline const_reference operator *( void ) const 
    {
        return *m_pointer;
    }

    inline pointer operator &( void ) const
    {
        return m_pointer;
    }

    inline operator bool( void ) const
    {
        return ( m_pointer != nullptr );
    }

private:
    pointer m_pointer;

    inline void IncRef( pointer & in_pointer )
    {
        if( in_pointer == nullptr )
            return;

        /// recovery counter pointer 
        SDL_AtomicU32 *counter = reinterpret_cast<SDL_AtomicU32*>( reinterpret_cast<unsigned char*>( in_pointer ) - sizeof( SDL_AtomicU32 ) );
        
        /// increment the reference pointer by one 
        SDL_AddAtomicU32( counter, 1 ); 
    }

    inline bool DecRef( pointer & in_pointer )
    {
        /// already deleted
        if( in_pointer == nullptr )
            return true;

        /// get the pointer counter
        SDL_AtomicU32 *counter = reinterpret_cast<SDL_AtomicU32*>( reinterpret_cast<unsigned char*>( in_pointer ) - sizeof( SDL_AtomicU32 ) );
        
        /// decrement the pointer reference counter by one
        return ( SDL_AddAtomicU32( counter, -1 ) - 1 ) != 0; 
    }

    inline void DeleteThis( void )
    {
        if( m_pointer != nullptr )
        {
            reinterpret_cast<_t*>( m_pointer)->_t();
            m_pointer = nullptr;
        }
    }
};

#endif ///!__AUTO_POINTER_HPP__