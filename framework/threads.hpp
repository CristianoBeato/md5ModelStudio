
#ifndef __THREADS_HPP__
#define __THREADS_HPP__

#include <SDL3/SDL_thread.hpp>
#include <SDL3/SDL_mutex.hpp>

class crThread
{
public:
    crThread( void );
    ~crThread( void );

    void    Start( const char* in_name );
	void    Exit( const bool in_Wait );

	inline const bool IsPendingExit( void ) const 
    { 
        return m_pendingExit; 
    };

	static SDL_ThreadID GetCurrentThread( void );

protected:
    virtual void Run( void ) {}
    virtual void NotifyExit( void ) {}

private:
    volatile bool   m_pendingExit;
    SDL_ThreadID    m_threadID;
    SDL::Thread     m_threadHandle;

    static int      ThreadStaticEntryPoint( void* in_ptr );
};

class crWorker : public crThread
{
public:
    crWorker( void );
    ~crWorker( void );

    // Wake up worker 
    void    Wake( void );

    // Is worker idle ?
    bool    Idle( void ) const;
    
protected:
    virtual void Run( void ) override;
    virtual void DoWork( void ) {};

private:
    SDL::Semaphore  m_work;
};

#endif //!__THREADS_HPP__