#!/usr/bin/env python2

from gi.repository import GMS
import argparse

if __name__=="__main__":
    arg_parser = argparse.ArgumentParser(description='Compare two streams frame \
    by frame')
    arg_parser.add_argument('reference_uri')
    arg_parser.add_argument('compared_uri')
    arg_parser.add_argument("--tolerance", help='the tolerance for frame \
            difference, between 0 (completely tolerant) and 1 (intolerant)', nargs=1)
    arg_parser.add_argument("--verbosity", help="level of verbosity"\
        , choices = ["all","failing"])
    args = arg_parser.parse_args()

    tolerance = float(args.tolerance[0])
    GMS.init()
    comp = GMS.VideoStreamComparator()
    comp.set_reference_uri(args.reference_uri, 0, 0)
    comp.add_compared_uri(args.compared_uri, 0, 0)
    result = comp.compare_sync(tolerance, None)

    if args.verbosity == "all":
        comp.print_results_for_uri(args.compared_uri,
                GMS.VideoStreamComparatorReportLevel.PERCENTAGE_FAILING)
    elif args.verbosity == "failing": 
        comp.print_results_for_uri(args.compared_uri,
                GMS.VideoStreamComparatorReportLevel.PERCENTAGE_FAILING |
                GMS.VideoStreamComparatorReportLevel.FAILING_ONLY)
