//--------------------------------------------------------------------------------
/// @file mac.hh mac address parser.
//--------------------------------------------------------------------------------

#ifndef __WOL_MAC__
#define __WOL_MAC__

#include <ranges>
#include <string_view>
#include <vector>

namespace wol {
    //----------------------------------------------------------------------------
    /// Parse and keep Mac Address.
    /// - Insufficient support for split_view with clang 14.0.0-1ubuntu1.
    /// - Pipeline style is supported in std::ranges.
    /// - To delive and extend, remove final and not conposition but protected inhelit.
    ///   - class MacAddress final {}  -> class MackAddress : std::vector< u_char >{}
    //----------------------------------------------------------------------------

    using Mac = std::vector< u_char >;		///< vector keeping Mac Address.

    static constexpr int MacLen = 6;		///< Mac length in bytes.

    class MacAddress final {
	static constexpr int Hex = 16;		///< Mac is written in hexadecimal.

	Mac mac_;				///< vector keeping Mac Address.

    public:
	MacAddress( const char *p ){ parse( p ); }
	MacAddress( const std::string &s ): MacAddress( s.c_str()){}

	void parse( const char *p ){	/// Parse from "xx:xx..." to vector< u_char >.
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
		throw std::runtime_error( "illegal length of mac address." );
	}
	const auto data() const { return mac_; }
    };
}
#endif

//--------------------------------------------------------------------------------
// end of file <mac.hh>
//--------------------------------------------------------------------------------
