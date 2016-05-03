#!/usr/bin/perl

=head1 NAME

osv-bms-main.pl - This script is used to get the data from the OSV's BMS,
through serial port, and POST it to a server in a HTTP request in JSON format.

=head1 COPYRIGHT

Copyright 2016 Institut Mines-Télécom

osv-bms-main.pl is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

osv-bms-main.pl is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with osv-bms-main.pl. If not, see <http://www.gnu.org/licenses/>.

=head1 AUTHORS

Guillaume Le Gall <guillaume.legall@telecom-bretagne.eu>
Baptiste Gaultier <http://baptistegaultier.fr>

=head1 SEE ALSO

BMS protocol documentation - <https://github.com/telecombretagne/osv/blob/master/bms_reverse/bms_reverse.html>

=cut

use strict;
use warnings;

use LWP::UserAgent;

use Device::SerialPort qw( :PARAM :STAT 0.07 );

my $endpoint = "https://dweet.io:443/dweet/for/osv1";

my $user_agent = LWP::UserAgent->new;

# Useful variables
my @baudrates = ( 9600,   19200,  38400, 57600, 115200 );
my @parity    = ( "none", "even", "odd" );
my @databits  = ( 5,      6,      7, 8 );
my @stopbits  = ( 1,      2 );
my @handshake = ( "none", "rts",  "xoff" );

# Open port
my $port = Device::SerialPort->new( "/dev/ttyACM0", 0 )
  or die "Could not open port: " . $!;

# Configuration
$port->parity("none");
$port->databits(8);
$port->stopbits(1);
$port->baudrate(9600);
$port->read_char_time(10);
sleep 1;

# Catch sig int and close port before exiting.
$SIG{INT} = sub {
	$port->write("00\n");
	$port->close() or warn "Failed to close port: " . $!;
	undef $port;    # Frees object memory.
};

$port->write("01") or warn "Write failed\n";

my ( $count_in, $string_in );
my $str   = "";
my $time  = time - 10;
my @array = ();
while (1) {
	( $count_in, $string_in ) = $port->read(1);

	# TODO probaby not the optimal way to wait for new char.
	next if ( !$count_in );
	warn "Could not read" unless $count_in;
	if ( $string_in ne "\n" ) {
		$str .= $string_in;
	} else {
		if ( $str =~ /940(\d{2})/ ) {
			print "SOC = " . $1 . "\n";

			if ( time - $time > 10 ) {
				print "Will send SOC.\n";
				&sendRequestWithCells( $1, @array );
				$time = time;
			}
		} else {
			if ( $str =~ /^(\d\d)(\d{3})/ && $1 < 25 ) {

				$array[ $1 - 1 ] = $2 / 200;
				print "Cell " . $1 . " = " . $array[ $1 - 1 ] . "\n";
			}
		}
		$str = "";
	}
}

sub readPortStatus {
	my ( $blockingflags, $inbytes, $outbytes, $errflags ) = $port->status
	  or warn "could not get port status\n";

	print "blockingflags= " . $blockingflags . "\n";
	print "inbytes= " . $inbytes . "\n";
	print "outbytes= " . $outbytes . "\n";
	print "errflags= " . $errflags . "\n";

	die "Too many inbytes, wrong mode." if ( $inbytes > 1000 );
}

# TODO This is a copy/paste of sendRequest, with different request body. Should
# be improved/refactored.
sub sendRequestWithCells {
	my $soc   = shift;
	my @cells = @_;

	return if ( $cells[23] == 0 );

	my $request = HTTP::Request->new( POST => $endpoint );
	$request->header( 'content-type' => 'application/json' );

	# add POST data to HTTP request body
	my $post_data = '{ "state_of_charge":' . $soc . ",";
	for ( my $i = 0 ; $i < 24 ; $i++ ) {
		$post_data .= "\"cell_" . ( $i + 1 ) . "\":" . $cells[$i];
		if ( $i < 23 ) {
			$post_data .= ",";
		}
	}
	$post_data .= "}";

	print "Posting: " . $post_data . "\n";

	$request->content($post_data);

	my $response = $user_agent->request($request);
	if ( $response->is_success ) {
		my $message = $response->decoded_content;
		print "Received reply: $message\n";
	}
}

sub sendRequest {
	my $soc = shift;
	my $request = HTTP::Request->new( POST => $endpoint );
	$request->header( 'content-type' => 'application/json' );

	# add POST data to HTTP request body
	my $post_data = '{ "state_of_charge":' . $soc . '}';
	$request->content($post_data);

	my $response = $user_agent->request($request);
	if ( $response->is_success ) {
		my $message = $response->decoded_content;
		print "Received reply: $message\n";
	}
}
