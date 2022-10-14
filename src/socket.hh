//--------------------------------------------------------------------------------
/// @file socket.hh RAII object for socket.
//--------------------------------------------------------------------------------

#ifndef __WOL_SOCKET__
#define __WOL_SOCKET__

#include <stdexcept> 
#include <sys/socket.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace wol {
    //----------------------------------------------------------------------------
    /// interface of inet library.
    //----------------------------------------------------------------------------

    struct SocketIF {
	template< typename Func, typename... Args >
	auto dispatch( Func func, Args... args ) -> decltype( func( args... )){
	    return func( args... );
	}

	virtual int socket( int domain, int type, int protocol ) = 0;
	virtual int setsockopt( int sockfd, int level, int optname,
				const void *optval, socklen_t optlen ) = 0;
	virtual int close( int fd ) = 0;
    };

    //----------------------------------------------------------------------------
    /// concreate class of socket library.
    //----------------------------------------------------------------------------

    struct RealSocketIF: public SocketIF {
	virtual int socket( int domain, int type, int protocol ) override {
	    return dispatch( ::socket, domain, type, protocol );
	}
	virtual int setsockopt( int sockfd, int level, int optname,
				const void *optval, socklen_t optlen ) override {
	    return dispatch( ::setsockopt, sockfd, level, optname, optval, optlen );
	}
	virtual int close( int fd ) override {
	    return dispatch( ::close, fd );
	}
    } RSock;					///< Real Socket Interface
    
    //----------------------------------------------------------------------------
    /// mock class of socket library.
    //----------------------------------------------------------------------------

    struct MockSocketIF : public SocketIF {
	MOCK_METHOD( int, socket, ( int, int, int ), ( override ));
	MOCK_METHOD( int, setsockopt,
		     ( int, int, int, const void*, socklen_t ), ( override ));
	MOCK_METHOD( int, close, ( int ), ( override ));
    };

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
	SocketIF *sif_ = &RSock;

    public:
	Socket( int dom, int type, int prot ){
	    if (( fd_ = sif_->socket( dom, type, prot )) == SocketError ) [[ unlikely ]]
		throw std::runtime_error( "can not open socket" );
	}
	Socket( int d, int t, int p, int level, int opt ): Socket( d, t, p ){
	    if ( int v{ 1 }; sif_->setsockopt( fd_, level, opt,
					 &v, sizeof( v )) == SocketError ) [[ unlikely ]]
		throw std::runtime_error( "can't set option to socket" );
	};
	virtual ~Socket(){ sif_->close( fd_ ); }

	operator const int&() const { return fd_; }	/// implicit conversion.
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <socket.hh>
//--------------------------------------------------------------------------------
