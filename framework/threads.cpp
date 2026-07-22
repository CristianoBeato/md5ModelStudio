
#include "precompiled.hpp"
#include "threads.hpp"

crThread::crThread( void ) : 
    m_pendingExit( false ),
    m_threadID( 0 ),
    m_threadHandle( nullptr )
{
}

crThread::~crThread( void )
{
}

void crThread::Start(const char *in_name)
{
    m_threadHandle = SDL_CreateThread( ThreadStaticEntryPoint, in_name, this );
    if( !m_threadHandle )
        throw std::runtime_error( SDL_GetError() );

    m_threadID = SDL_GetThreadID( m_threadHandle );
}

void crThread::Exit(const bool in_wait)
{
    m_pendingExit = true;

    /// Signal thread work that we are done
    NotifyExit();

    /// if we don't neet to wait for the thread just leave
    if ( !in_wait )
        return; 

    /// Make sure that we are not calling a exit from the inside of the thread
    if( m_threadID != GetCurrentThread() )
        SDL_WaitThread( m_threadHandle, nullptr );
}

int crThread::ThreadStaticEntryPoint( void * in_ptr )
{
    crThread* Thread = reinterpret_cast<crThread*>( in_ptr );
    if( Thread )
        Thread->Run();

    return 0;
}

crWorker::crWorker( void ) : crThread()
{
    // wait for a work cicle
    m_work.Create( 0 );
}

crWorker::~crWorker( void )
{
    m_work.Destroy();
}

void crWorker::Wake(void)
{
    m_work.Signal();
}

bool crWorker::Idle(void) const
{
    return m_work.GetValue() == 0;
}

void crWorker::Run(void)
{
    while ( !IsPendingExit() )
    {
        /// Wait for wakeup
        m_work.Wait();
        DoWork();
    }
}
