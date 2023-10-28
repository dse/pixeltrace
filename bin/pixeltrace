#!/usr/bin/env perl
use warnings;
use strict;
use Getopt::Long;
use POSIX qw(ceil);
use List::Util qw(max min);
use Data::Dumper qw(Dumper);

our $output_format;
our $input_format;

Getopt::Long::Configure('gnu_getopt');
Getopt::Long::GetOptions('output-format=s' => \$output_format,
                         'input-format=s' => \$input_format);

my $filename = shift(@ARGV);
our $bmp_data = get_bmp_data($filename);
my $parsed = parse_bmp_data($bmp_data);
if (!defined $parsed) {
    exit(0);
}
my $width = $parsed->{width};
my $height = $parsed->{height};
my $eps = generate_eps($parsed);
print($eps);

sub generate_eps {
    my $data = shift;
    my $width  = defined $data ? $data->{width} : undef;
    my $height  = defined $data ? $data->{height} : undef;
    my $bits  = defined $data ? $data->{bits} : undef;
    $width //= 0;
    $height //= 0;
    $bits //= 0;
    my $eps = <<"END";
%!PS-Adobe-3.0 EPSF-3.0
%%Creator: pixeltrace
%%LanguageLevel: 2
%%BoundingBox: 0 0 ${width} ${height}
%%HiResBoundingBox: 0 0 ${width}.000000 ${height}.000000
%%Pages: 1
%%EndComments
%%Page: 1 1
END
    if (!$width || !$height || !$bits) {
        return $eps;
    }
    for (my $y = 0; $y < scalar @$bits; $y += 1) {
        my $line = $bits->[$y];
        my $y1 = $height - $y;
        my $y2 = $height - $y - 1;
        my $x1 = 0;
        while (length $line) {
            $line =~ s{^0+}{};
            my $white = length($& // '');
            $x1 += $white;
            $line =~ s{^1+}{};
            my $dark = length($& // '');
            my $x2 = $x1 + $dark;
            $eps .= sprintf("%d %d moveto %d %d lineto %d %d lineto %d %d lineto closepath  0 setlinewidth  fill\n",
                            $x1, $y1, $x1, $y2, $x2, $y2, $x2, $y1);
            $x1 += $dark;
        }
    }
    return $eps;
}

sub parse_bmp_data {
    my ($bmp_data) = @_;
    my $ofs = 0;
    my ($signature, $bytes, undef, $data_offset, $size, $width, $height, $planes, $bpp,
        $compression, $image_size, $x_px_per_m, $y_px_per_m, $colors_used,
        $important_colors, $data) = unpack('A2VVVVVVvvVVVVVVA*', $bmp_data);
    my $num_colors = 2 ** $bpp;
    $bmp_data = $data;
    if (!defined $bmp_data) {
        return;
    }
    $ofs += 54;
    die("invalid signature in BMP data") if ($signature ne 'BM');
    die("autotrace expects 1 plane") if ($planes != 1);
    die("autotrace expects 1 bit per pixel") if ($bpp != 1);
    die("autotrace expects uncompressed data") if ($compression != 0);
    my $bits_per_row = ceil($width / 32) * 32;
    for (my $i = 0; $i < length($bmp_data); $i += 1) {
        my $byte = ord(substr($bmp_data, $i, 1));
    }
    my @colors;
    my @bright;
    if ($bpp < 8) {
        for (my $i = 0; $i < $num_colors; $i += 1) {
            my ($red, $green, $blue, undef, $data) = unpack('CCCCA*', $bmp_data);
            $bmp_data = $data;
            if (!defined $bmp_data) {
                return;
            }
            $ofs += 4;
            push(@colors, [$red, $green, $blue]);
            my $bright = 0.2126 * $red + 0.7152 * $green + 0.0722 * $blue;
            push(@bright, $bright);
        }
    }
    my $interstitial_data;
    {
        my $bytes = $data_offset - $ofs;
        if ($bytes > 0) {
            $interstitial_data = substr($bmp_data, 0, $bytes);
            $bmp_data = substr($bmp_data, $bytes);
            if (!defined $bmp_data) {
                return;
            }
        }
    }
    my $bits = unpack('B*', $bmp_data);
    my @bits;
    while (length($bits) >= $bits_per_row && scalar @bits < $height) {
        push(@bits, substr($bits, 0, $bits_per_row));
        $bits = substr($bits, $bits_per_row);
    }
    @bits = map { substr($_, 0, $width) } reverse @bits;

    my $min_bright = min(@bright);
    my ($dark_idx) = grep { $bright[$_] == $min_bright } (0 .. $#bright);
    if ($dark_idx == 0) {
        @bits = map { bit_inverse($_) } @bits;
    }
    return {
        width => $width,
        height => $height,
        colors => \@colors,
        bright => \@bright,
        bits => \@bits,
    };
}

sub get_bmp_data {
    my ($filename) = @_;
    my $fh;
    open($fh, '<', $filename) || die("$filename: $!");
    binmode($fh) or die("$filename: $!");
    local $/ = undef;
    my $data = <$fh>;
    return $data;
}

sub bit_inverse {
    my ($bits) = @_;
    $bits =~ s{0}{x}g;
    $bits =~ s{1}{0}g;
    $bits =~ s{x}{1}g;
    return $bits;
}
