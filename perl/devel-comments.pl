#!/usr/bin/env perl

# Run with -MDevel::Comments

### Pre-loop with line number (<here>)

### Timestamp (<now>)

my $a = 3;
my $b = 4;
# Need a colon : if you want an expression evaluated:
### Eval an expresion: $a+$b

### Fast loop, shows no progress bars

# No progress bars printed if loop is too fast
for (1..10) { ### |===>    | %
}

### Longer loop. Note, need 3 progress characters (Here, ===)
for (1..1000000) { ### |===>    | %
}


### Post-loop <here> at <now>

### Dump a tied object with tied()
use DBI;
my $db =   shift || 'mysql';
my $host = shift || 'russelllab.org';
my $user = shift || 'anonymous';
my $dbh = DBI->connect(
    'dbi:mysql:' . join(';', "database=$db", "host=$host"),
    $user,
    );

# Note the use of tied, the dumping itself is handled by Devel::Comments
### dbh: tied(%$dbh)
