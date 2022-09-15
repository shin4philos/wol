//--------------------------------------------------------------------------------
/// @file mac.hh mac address parser.
//--------------------------------------------------------------------------------

#ifndef __WOL_MAC__
#define __WOL_MAC__

#include <ranges>
#include <string_view>
#include <vector>

namespace wol {
    class MacAddress final {				/// Mac address parser.
	static constexpr int MacLen = 6;		///< Mac address length.
	static constexpr int Hex = 16;			///< base 16

	std::vector< u_char > mac_;

    public:
	MacAddress( const char *p ){ parse( p ); }
	MacAddress( const std::string &s ){ parse( s.c_str()); }

	void parse( const char *p ){	/// Parse from c_str to vector< u_char >.
	    if ( !p )
		throw std::runtime_error( "null mac address." );

	    using std::ranges::split_view;
	    using std::string_view;
	    try {
		for ( const auto octet : split_view{ string_view( p ), ':' }){
		    auto it = octet.begin();
		    mac_.push_back( std::stoi( std::string{ *it++, *it }, nullptr, Hex ));
		}
	    }
	    catch( const std::exception &e ){
		throw std::runtime_error( "syntax error in mac address." );
	    }
	    if ( mac_.size() != MacLen )
		throw std::runtime_error( "mac address too short." );
	}
	operator std::vector< u_char >&(){ return mac_; };
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <mac.hh>
//--------------------------------------------------------------------------------
