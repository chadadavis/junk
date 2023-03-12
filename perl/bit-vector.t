#!/usr/bin/env perl

use Test::More 'no_plan';
use Moose::Autobox;

my @sedges = qw/worst ok good great/;
# Map each element name to its index
my $sedge_index = { map { $sedges[$_] => $_ } (0..$#sedges) };

use Bit::Vector::Overload;
Bit::Vector->Configuration("in=enum,out=bin");

sub bitstr2indices {
    my $str = shift;
    # Bit str has index 0 on the right, so reverse it
    my @bits =  split //, reverse $str;
    my @indices = grep { $bits[$_] } (0..$#bits);
    return @indices;
}


my $bitvector = Bit::Vector->new(scalar @sedges);
# Set all to enabled, and count down to empty set
$bitvector->Fill;

my $masks = [];
$masks->push(conflict_mask($sedge_index, [qw/great ok/]));

# $bitvector nows it's current values, can just push that object on stack

# Count down through all subsets of sub-edges

sedge_set: for (; $bitvector; $bitvector--) {
     print "$bitvector : ";
     
     foreach (@$masks) {
         # See if the mask is a subset of the $bitvector (i.e. if it applies)
         if ($_ < $bitvector) { print "masked\n"; }
         next sedge_set if $_ < $bitvector;
     }
     
     my @names = reverse @sedges[bitstr2indices("$bitvector")];
     print "names : @names\n";
     
     # Pre-decrement counter, skipping the null set at the end
}; # while --$bitvector;

sub conflict_mask {
    my ($map, $conflict) = @_;
    my $indices = $map->slice($conflict);
    my $mask = Bit::Vector->new($map->keys->length);
    $mask->from_Enum($indices->join(','));
    print "mask:$mask:\n";
    return $mask;
}

__END__







use SBG::U::Test 'float_is';
use Carp;
use Data::Dumper;
use FindBin qw/$Bin/;

use SBG::U::Log qw/log/;
$SIG{__DIE__} = \&confess;
my $DEBUG;
$DEBUG = 1;
log()->init('TRACE') if $DEBUG;
$File::Temp::KEEP_ALL = $DEBUG;


use Trav1;
use SBG::NetworkIO::csv;

# Load up a network
my $file = "$Bin/data/simple_network-s.csv";
my $io = new SBG::NetworkIO::csv(file=>$file);
my $net = $io->read;


# For the sake of testing, define some incompatible sets of templates.  Anything
# in the same set is all not compatible.  NB alternative templates for a single
# edge are implicitly mutually incompatible.

use Graph::UnionFind;
# Set of sets of templates
my $uf = new Graph::UnionFind; 
# In this simple example, each template has a numeric index
my @templ;
foreach my $e ($net->edges) {
    my ($u, $v) = @$e;
    # Names of templates for this edge
    my @templ_ids = sort $net->get_edge_attribute_names($u, $v);
    # Assign a numeric ID to each template (extracted from name), for simplicity
    foreach my $t (@templ_ids) {
        my ($id) = $t =~ /(\d+)/;
        $templ[$id] = $t;
        # Put each template into the union-find data structure. 
        # Initially each is in its own set, i.e. no incompatibilities yet
        $uf->add($t);
    }
}

# Define some arbitrary incompatibilities by putting templates into same set
$uf->union($templ[1], $templ[5]); 
$uf->union($templ[1], $templ[2]); 
$uf->union($templ[2], $templ[5]);
$uf->union($templ[1], $templ[6]);
$uf->union($templ[2], $templ[6]);

# $uf->union($templ[4], $templ[7]);
# $uf->union($templ[4], $templ[8]); 
# $uf->union($templ[4], $templ[9]); 
# $uf->union($templ[3], $templ[9]); 

# Track answers
my %answers;
# my %expect = "A B 1;A B 2;A B C 1 3;A B C 2 3;A B C 3 5;A B D 1 4;A B D 2 4;B C 5;B C 6;B C D 4 6;B D 4";

my %expected = (
    "A B 1" => 1,
    "A B 2" => 1,
    "A C 3" => 1,
    "A B C 1 3" => 1,
    "A B C 2 3" => 1,
    "A B C 3 5" => 1,
    "A B C 3 6" => 1,
#     "A B D 1 4" => 1,
#     "A B D 2 4" => 1,
    "B C 5" => 1,
    "B C 6" => 1,
#     "B C D 4 5" => 1,
#     "B C D 4 6" => 1,
#     "B C D 5 7" => 1, 
#     "B C D 5 8" => 1,
#     "B C D 6 7" => 1,
#     "B C D 6 8" => 1,
#     "B D 4" => 1,
    );

# Create a traversal
my $trav = new SBG::Traversal(graph=>$net, 
                              assembler=>new TestAssembler,
    );

$trav->traverse;

is(scalar(keys %answers), 11, "11 Covering solutions from traversal");

foreach (sort keys %answers) {
    ok($expected{$_}, "Solution was expected: $_");
    delete $expected{$_};
}
is(scalar(keys %expected), 0,
   "Missing solutions? " . join(',', keys %expected));

exit;


################################################################################

package TestAssembler;
use Moose;

# with 'SBG::AsseblerI';

# In these callbacks, the default $state will just be a HashRef
sub solution {
    my ($self, $state, $g, $nodecover, $templates) = @_;
    return unless defined($nodecover);
    my @ids;
    foreach my $t (@$templates) {
        push @ids, ex_id($t);
    }
    @$nodecover = sort @$nodecover;
    @ids = sort @ids;
#     @$templates = sort @$templates;
#     print STDERR 
#         "Solution: ",
#         "Nodes @$nodecover, Templates: @ids : \n@$templates\n";

    # Reset running list of altids used in the current solution
    $state->{solutions} = [];
    # Append this solution to total answers
    $answers{"@$nodecover @ids"} = 1;
}


sub score {
    my ($self, $graph, $altid) = @_;
    return 1;
}


sub test {
    my ($self, $state, $graph, $u, $v, $altid) = @_;
    # Our test index of this template
#     my $templ_id = ex_id($alt_id);

    # What other templates already being used
    foreach my $other (@{$state->{'solutions'}}) {
#         my $other_id = ex_id($other);
#         print STDERR "== $templ_id vs $other_id\n";
#         if ($uf->same($templ[$templ_id], $templ[$other_id])) {
        if ($uf->same($altid, $other)) {
#             print STDERR "== Incompatible with $other_id\n";
            return;
        }
    }
    # Add this template to progressive solution (list of templates used)
    push @{$state->{'solutions'}}, $altid;
    # Arbitrary placement score
    return 1;
}

sub ex_id {
    my $name = shift;
    $name =~ /(\d+)/;
    my $id = $1;
}

