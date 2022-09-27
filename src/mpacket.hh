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

#include "socket.hh"
#include "mac.hh"

namespace wol {
    //----------------------------------------------------------------------------
    /// interface of socket library.
    //----------------------------------------------------------------------------

    using Packet = std::vector< u_char >;

    struct SocketIF {
	virtual int aton( sockaddr_in & ) = 0;
	virtual int send( Socket &, const Packet &, sockaddr_in & ) = 0;
    };

    //----------------------------------------------------------------------------
    /// concreate class of socket library.
    //----------------------------------------------------------------------------

    struct RealSocketIF : public SocketIF {
	virtual int aton( sockaddr_in &addr ){
	    return inet_aton( "255.255.255.255",
			      reinterpret_cast< in_addr * >( &addr.sin_addr.s_addr ));
	}
	virtual int send( Socket &socket,
			    const Packet &packet, sockaddr_in &addr ){
	    return sendto( socket, packet.data(), packet.size(), 0,
			   reinterpret_cast< sockaddr * >( &addr ), sizeof( addr ));
	}
    };
    static RealSocketIF RealIF{};

    //----------------------------------------------------------------------------
    /// Make and send magic packet.  
    /// SocketIF の差し替えは unit test でのみ使うので、エラーチェックは assert
    /// ですませる。
    /// @note Magic Packet のデータ部は 0xff が 6 回繰り返された後、送信先の MAC
    ///       address が16 回繰り返される。（データ長は 102 byte）
    //----------------------------------------------------------------------------

    class MagicPacket final {
	static constexpr int Header = 6;	///< magic packet header length
	static constexpr int Repeat = 16;	///< repeat times of mac
	static constexpr int Port   = 9;	///< port number of discard

	Packet packet_{};
	SocketIF *sockif_{};

    public:
	MagicPacket( const MacAddress &mac, SocketIF *sockif = &RealIF )
					: packet_( Header, 0xff ), sockif_( sockif )
	{
	    assert( sockif_ );
	    auto v = mac.data();
	    for ( int i = 0; i < Repeat; ++i )
		packet_.insert( packet_.end(), v.begin(), v.end());
	}

	void broadcast() const {		/// make and broadcast magic packet
	    Socket sock( AF_INET, SOCK_DGRAM, 0, SOL_SOCKET, SO_BROADCAST );
	    sockaddr_in addr = { .sin_family = AF_INET, .sin_port = htons( Port )};
	    if ( !sockif_->aton( addr )) [[ unlikely ]]
		throw std::runtime_error( "inet_aton: can not convert ??" );
	    else if ( sockif_->send( sock, packet_, addr ) == SocketError ) [[ unlikely ]]
		throw std::runtime_error( strerror( errno ));
	}
	const auto *packet() const { return packet_.data(); };
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <mpacket.hh>
//--------------------------------------------------------------------------------
