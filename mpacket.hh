//--------------------------------------------------------------------------------
/// @file mpacket.hh send magic packet for nfs client machine
//--------------------------------------------------------------------------------

#ifndef __WOL_MPACKET__
#define __WOL_MPACKET__

#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "socket.hh"
#include "mac.hh"

namespace wol {
    constexpr int Header = 6;		// magic packet header length
    constexpr int Repeat = 16;		// repeat times of mac
    constexpr int Port   = 9;		// port number of discard

    //----------------------------------------------------------------------------
    /// Make and send magic packet
    ///
    /// Magic Packet のデータ部は 0xff が 6 回繰り返された後、送信先の MAC address が
    /// 16 回繰り返される。（データ長は 102 byte）
    //----------------------------------------------------------------------------

    class MagicPacket {
	u_char packet_[ Header + MacLen * Repeat ]{ "\xff\xff\xff\xff\xff\xff" };

	void check( bool error, const char *message = nullptr ) const {
	    if ( error )
		throw std::runtime_error( message ? message : strerror( errno ));
	}

    public:
	MagicPacket( const std::vector< u_char > &mac ){
	    auto *p = &packet_[ Header ];
	    for ( auto i{ 0 }; i < Repeat ; ++i, p += MacLen )
		std::memcpy( p, mac.data(), MacLen );
	}

	const auto *packet(){ return packet_; };

	const auto address( sockaddr_in &addr ) const {
	    return inet_aton( "255.255.255.255",
			      reinterpret_cast< in_addr * >( &addr.sin_addr.s_addr ));
	};
	const auto option( Socket &socket ) const {
	    int val{ 1 };
	    return setsockopt( socket, SOL_SOCKET, SO_BROADCAST,
			       reinterpret_cast< char * >( &val ), sizeof( val ));
	};
	const auto send( Socket &socket, sockaddr_in &addr ) const {
	    return sendto( socket, packet_, sizeof( packet_ ), 0,
			   reinterpret_cast< sockaddr * >( &addr ), sizeof( addr ));
	};

	void send() const {
	    Socket sock( AF_INET, SOCK_DGRAM, 0 );
	    sockaddr_in addr = { AF_INET, htons( Port )};
	    check( address( addr )    == 0,"inet_aton: can not convert ??" );
	    check( option( sock )     == SocketError );
	    check( send( sock, addr ) == SocketError );
	}
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <mpacket.hh>
//--------------------------------------------------------------------------------
