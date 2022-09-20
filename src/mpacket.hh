//--------------------------------------------------------------------------------
/// @file mpacket.hh send magic packet for nfs client machine
//--------------------------------------------------------------------------------

#ifndef __WOL_MPACKET__
#define __WOL_MPACKET__

#include <cstring>
#include <vector>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "socket.hh"
#include "mac.hh"

namespace wol {
    //----------------------------------------------------------------------------
    /// Make and send magic packet
    ///
    /// Magic Packet のデータ部は 0xff が 6 回繰り返された後、送信先の MAC address が
    /// 16 回繰り返される。（データ長は 102 byte）
    //----------------------------------------------------------------------------

    class MagicPacket final {
	static constexpr int Header = 6;	// magic packet header length
	static constexpr int Repeat = 16;	// repeat times of mac
	static constexpr int Port   = 9;	// port number of discard

	std::vector< u_char > packet_;

	const auto address( sockaddr_in &addr ) const {
	    return inet_aton( "255.255.255.255",
			      reinterpret_cast< in_addr * >( &addr.sin_addr.s_addr ));
	};
	const auto send( Socket &socket, sockaddr_in &addr ) const {
	    return sendto( socket, packet_.data(), size( packet_ ), 0,
			   reinterpret_cast< sockaddr * >( &addr ), sizeof( addr ));
	};

    public:
	MagicPacket( const std::vector< u_char > &mac ): packet_( Header, 0xff ){
	    for ( int i = 0; i < Repeat; ++i )
		packet_.insert( packet_.end(), mac.begin(), mac.end());
	}

	const auto *packet() const { return packet_.data(); };

	void broadcast() const {		/// make and broadcast magic packet
	    Socket sock( AF_INET, SOCK_DGRAM, 0, SOL_SOCKET, SO_BROADCAST );
	    sockaddr_in addr = { .sin_family = AF_INET, .sin_port = htons( Port )};
	    if ( !address( addr )) [[ unlikely ]]
		throw std::runtime_error( "inet_aton: can not convert ??" );
	    else if ( send( sock, addr ) == SocketError ) [[ unlikely ]]
		throw std::runtime_error( strerror( errno ));
	}
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <mpacket.hh>
//--------------------------------------------------------------------------------
