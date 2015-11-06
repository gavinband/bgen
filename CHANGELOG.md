History
====
6 Nov 2015
----
Major changes in revision 8762b40dedae:

1. I’ve changed the behaviour of BGEN v1.2 with respect to samples with missing data: they are now stored with dummy zero probabilities.  The spec is now in 'beta' which means I don’t have any other planned changes to make; unless major issues are uncovered this will be the final version of the format.

2. I’ve revamped the setter api of parse_probability_data somewhat.  It is documented in the code and here [on the wiki](https://bitbucket.org/gavinband/bgen/wiki/The_Setter_API).  The main breaking changes are:
- Renamed operator() to set_value(), and given it an index argument; I think these make the API more consistent.
- Added an initial ploidy argument to set_number_of_entries() as requested.  (The type of data - phased or unphased - is already reported in the order_type argument so I don’t think another argument is needed).
- Added two new method calls, which are optional: set_min_max_ploidy() (useful for setting storage) and finalise().  See the docs for info.

3. I’ve also got rid of the max_id_size option to write_snp_identifying_data().  (This is now not needed because writing BGEN v1.0 files is no longer supported.)

4. I’ve also added some test code (using the [catch framework](https://github.com/philsquared/catch), which seems pretty good).  Tests are not exhaustive but hopefully a start.

5. I've removed some code warnings - thanks to Robert V. Baron of [Mega2](https://watson.hgen.pitt.edu/docs/mega2_html/mega2.html) for testing this code.

23 Sep 2015
----
First version, based on qctool implementation.