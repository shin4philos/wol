//--------------------------------------------------------------------------------
/// @file wol_test.cc
//--------------------------------------------------------------------------------

#include <vector>
#include <socket.hh>
#include <mac.hh>
#include <mpacket.hh>

#include <gtest/gtest.h>

using namespace wol;
using std::runtime_error;

TEST( SocketTest, NormalParameter ){
    EXPECT_NE( Socket( AF_INET, SOCK_DGRAM, 0 ), SocketError );
    EXPECT_NE( Socket( AF_INET, SOCK_DGRAM, 0, SOL_SOCKET, SO_BROADCAST ), SocketError );
}
TEST( SocketTest, IllegalParameter ){
    EXPECT_THROW( Socket( -1, SOCK_DGRAM, 0 ), runtime_error );
    EXPECT_THROW( Socket( -1, SOCK_DGRAM, 0, SOL_SOCKET, SO_BROADCAST ), runtime_error );
}

TEST( MacAddressTest, NormalParameter ){
    EXPECT_NO_THROW( MacAddress( "12:34:56:78:9a:bc" ));
    EXPECT_NO_THROW( MacAddress( std::string{ "12:34:56:78:9a:bc" }));
    EXPECT_EQ( MacAddress{ "11:22:33:44:55:66" }.data().size(), MacLen );
}
TEST( MacAddressTest, IllegalParameter ){
    EXPECT_THROW( MacAddress( nullptr ), runtime_error );
    EXPECT_THROW( MacAddress( "" ), runtime_error );
    EXPECT_THROW( MacAddress( std::string{}), runtime_error );
    EXPECT_THROW( MacAddress( "xyz" ), runtime_error );
    EXPECT_THROW( MacAddress( "12" ), runtime_error );
    EXPECT_THROW( MacAddress( "12:34" ), runtime_error );
    EXPECT_THROW( MacAddress( "12-34-56-78-9a=bc=de" ), runtime_error );
    EXPECT_THROW( MacAddress( "12:34:56:78:9a:bc:de" ), runtime_error );
    EXPECT_THROW( MacAddress( "sa:sa:ki:sh:in:su" ), runtime_error );
}

//--------------------------------------------------------------------------------
// end of file <wol_test.hh>
//--------------------------------------------------------------------------------