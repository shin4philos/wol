//--------------------------------------------------------------------------------
/// @file socket.hh RAII object for socket.
//--------------------------------------------------------------------------------

#ifndef __WOL_SOCKET__
#define __WOL_SOCKET__

#include <stdexcept> 
#include <sys/socket.h>
#include <unistd.h>

namespace wol {
    //----------------------------------------------------------------------------
    /// RAII object for socket file descriptor.
    /// 
    /// - Socket is always effective. Because Socket throw exception if fail
    ///   to create file descriptor.
    /// - Socket is noncopyable and nonmovable.
    //----------------------------------------------------------------------------

    constexpr int SocketError = -1;		///< socket error, errno not set.

    class Socket {
	int fd_{ SocketError };			///< socket file descriptor.

    public:
	Socket( int domain, int type, int protocol ){
	    if (( fd_ = socket( domain, type, protocol )) == SocketError )[[unlikely]]
		throw std::runtime_error( "can not open socket" );
	}
	~Socket(){ close( fd_ ); }
	Socket()                           = delete;
	Socket( const Socket& )            = delete;
	Socket &operator=( const Socket& ) = delete;
	Socket( Socket &&r )               = delete;
	Socket &operator=( Socket &&r )    = delete;

	operator const int() const { return fd_; }
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <socket.hh>
//--------------------------------------------------------------------------------
