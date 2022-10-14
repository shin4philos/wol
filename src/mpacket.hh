//--------------------------------------------------------------------------------
/// @file mpacket.hh send magic packet for nfs client machine
//--------------------------------------------------------------------------------

#ifndef __WOL_MPACKET__
#define __WOL_MPACKET__

#include <cassert>
#include <cstring>
#include <vector>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "socket.hh"
#include "mac.hh"

namespace wol {
    //----------------------------------------------------------------------------
    /// interface of inet library.
    //----------------------------------------------------------------------------

    struct InetIF {
	template< typename Func, typename... Args >
	auto dispatch( Func func, Args... args ) -> decltype( func( args... )){
	    return func( args... );
	}

	virtual int inet_aton( const char *cp, in_addr *inp ) = 0;
	virtual ssize_t sendto( int sockfd, const void *buf, size_t len, int flags,
				const sockaddr *dest_addr, socklen_t addrlen ) = 0;
    };

    //----------------------------------------------------------------------------
    /// concreate class of socket library.
    //----------------------------------------------------------------------------

    struct RealInetIF: public InetIF {
	virtual int inet_aton( const char *cp, in_addr *inp ) override {
	    return dispatch( ::inet_aton, cp, inp );
	}
	virtual ssize_t sendto( int sockfd, const void *buf, size_t len, int flags,
				const sockaddr *dest_addr, socklen_t addrlen ) override {
	    return dispatch( ::sendto, sockfd, buf, len, flags, dest_addr, addrlen );
	}
    } RInet;					///< Real Inet Interface

    //----------------------------------------------------------------------------
    /// mock class of socket library.
    //----------------------------------------------------------------------------

    struct MockInetIF : public InetIF {
	MOCK_METHOD( int, inet_aton, ( const char*, in_addr* ), ( override ));
	MOCK_METHOD( ssize_t, sendto, ( int, const void*, size_t, int,
					const sockaddr*, socklen_t ), ( override ));
    };

    //----------------------------------------------------------------------------
    /// Make and send magic packet.  
    /// SocketIF の差し替えは unit test でのみ使うので、エラーチェックは assert
    /// ですませる。
    /// @note Magic Packet のデータ部は 0xff が 6 回繰り返された後、送信先の MAC
    ///       address が16 回繰り返される。（データ長は 102 byte）
    //----------------------------------------------------------------------------

    using Packet = std::vector< u_char >;

    class MagicPacket final {
	static constexpr int Header = 6;	///< magic packet header length
	static constexpr int Repeat = 16;	///< repeat times of mac
	static constexpr int Port   = 9;	///< port number of discard

	Packet packet_{};
	InetIF *iif_{};

	int aton( sockaddr_in &addr ) const {
	    return iif_->inet_aton( "255.255.255.255",
			reinterpret_cast< in_addr * >( &addr.sin_addr.s_addr ));
	}
	int sendto( int fd, sockaddr_in &addr ) const {
	    return iif_->sendto( fd, packet_.data(), packet_.size(), 0,
				 reinterpret_cast< sockaddr * >( &addr ), sizeof( addr ));
	}

    public:
	explicit MagicPacket( const MacAddress &mac, InetIF *iif = &RInet )
					: packet_( Header, 0xff ), iif_( iif )
	{
#ifndef UNIT_TEST	// Only UNIT_TEST and Debug mode, you can set your own SocketIF.
	    assert( iif_ == &RInet &&
		    "You can set your own SocketIF only for UNIT_TEST." );
#endif
	    auto v = mac.data();
	    for ( int i = 0; i < Repeat; ++i )
		packet_.insert( packet_.end(), v.begin(), v.end());
	}

	void broadcast() const {		/// make and broadcast magic packet
	    Socket sock( AF_INET, SOCK_DGRAM, 0, SOL_SOCKET, SO_BROADCAST );
	    sockaddr_in addr = { .sin_family = AF_INET, .sin_port = htons( Port )};
	    if ( !aton( addr )) [[ unlikely ]]
		throw std::runtime_error( "inet_aton: can not convert ??" );
	    else if ( sendto( sock, addr ) == SocketError ) [[ unlikely ]]
		throw std::runtime_error( strerror( errno ));
	}
	[[ maybe_unused ]] const auto *packet() const { return packet_.data(); };
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <mpacket.hh>
//--------------------------------------------------------------------------------
