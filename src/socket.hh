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

    static constexpr int SocketError = -1;	///< socket error, errno not set.

    class Socket {
	int fd_{ SocketError };			///< socket file descriptor.

    public:
	Socket( int domain, int type, int protocol ){
	    if (( fd_ = socket( domain, type, protocol )) == SocketError ) [[ unlikely ]]
		throw std::runtime_error( "can not open socket" );
	}
	Socket( int d, int t, int p, int level, int opt ): Socket( d, t, p ){
	    if ( int v{ 1 }; setsockopt( fd_, level, opt,
					 &v, sizeof( v )) == SocketError ) [[ unlikely ]]
		throw std::runtime_error( "can't set option to socket" );
	};
	virtual ~Socket(){ close( fd_ ); }

	operator int() const { return fd_; }		/// implicit conversion.
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <socket.hh>
//--------------------------------------------------------------------------------
