//--------------------------------------------------------------------------------
// wol.cc : wake on lan, send magic packet for nfs client machine
//--------------------------------------------------------------------------------

#include <iostream>
#include <stdexcept>

#include "mac.hh"
#include "mpacket.hh"

using namespace wol;

//--------------------------------------------------------------------------------
/// usage show usage.
//--------------------------------------------------------------------------------

void usage(){
    std::cerr << "usage: wol mac" << std::endl;
    std::cerr << "  mac: xx:xx:xx:xx:xx:xx" << std::endl;
}

//--------------------------------------------------------------------------------
/// main parse mac address, send magic packet.
//--------------------------------------------------------------------------------

int main( int argc, char *argv[]){
    try {
	if ( argc < 2 ){
	    usage();
	    throw std::runtime_error( "no mac address." );
	}
	MagicPacket{ MacAddress{ argv[ 1 ]}}.send();
    }
    catch( const std::exception &e ){
	std::cerr << argv[ 0 ] << ": error: " << e.what() << std::endl;
	exit( 1 );
    }
}

//--------------------------------------------------------------------------------
// end of file <wol.cc>
//--------------------------------------------------------------------------------
