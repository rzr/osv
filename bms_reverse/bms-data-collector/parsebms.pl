
=head1 NAME

parsebms.pl - This script is a previous version of osv-bms-main.pl.
For details see osv-bms-main.pl.

=cut

use strict;
use warnings;
use LWP::UserAgent;

my $endpoint = "https://dweet.io:443/dweet/for/osv1";

my $user_agent = LWP::UserAgent->new;

my $filename = 'bms.log';
my $soc = 0;

my $last_api_call = 0;

if(open(my $fh, '<:encoding(UTF-8)', $filename)) {
	while (my $row = <$fh>) {
		chomp $row;
		# Deal with voltage cells
		# if ($row =~ /^01/) { print "cell_1:" . substr($row,2) ."\n"; }
		# Deal with
		if ($row =~ /^94/) {
			if($soc > 10) {
				$soc = 0;
				# print "soc:" . substr($row,2) . "\n";
				# send a message every 10 seconds
				if(time > ($last_api_call + 10)) {
					my $request = HTTP::Request->new(POST => $endpoint);
					$request->header('content-type' => 'application/json');

					# add POST data to HTTP request body
					my $post_data = '{ "state_of_charge":' . substr($row,2)*1 . '}';
					$request->content($post_data);

					my $response = $user_agent->request($request);
					if ($response->is_success) {
					   my $message = $response->decoded_content;
					   print "Received reply: $message\n";
						$last_api_call = time();
					}
					else {
					    print "HTTP POST error code: ", $response->code, "\n";
					    print "HTTP POST error message: ", $response->message, "\n";
					}
				}
			}
			$soc++;
		}
	}
} else {
	warn "Could not open file '$filename' $!";
}
