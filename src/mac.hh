//--------------------------------------------------------------------------------
/// @file mac.hh mac address parser.
//--------------------------------------------------------------------------------

#ifndef __WOL_MAC__
#define __WOL_MAC__

#include <ranges>
#include <string_view>
#include <vector>

namespace wol {

    const int MacLen = 6;				/// Mac address length.

    //----------------------------------------------------------------------------
    /// Mac address parser.
    ///
    /// - Parse mac address "xx:xx:xx:xx:xx:xx" to vectyor< u_chat >.
    //----------------------------------------------------------------------------

    struct MacAddress : public std::vector< u_char >{
	MacAddress( const char *p ){ parse( p ); }

	void parse( const char *p ){		/// Parse from c_str to vector< u_char >.
	    using std::ranges::split_view;
	    using std::string_view;
	    try {
		for ( const auto octet : split_view{ string_view( p ), ':' }){
		    auto it = octet.begin();
		    push_back( std::stoi( std::string{ *it++, *it }, nullptr, 16 ));
		}
	    }
	    catch( const std::exception &e ){
		throw std::runtime_error( "syntax error in mac address." );
	    }

	    if ( size() != MacLen )
		throw std::runtime_error( "mac address too short." );
	}
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <mac.hh>
//--------------------------------------------------------------------------------
