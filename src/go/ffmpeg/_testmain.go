package main

import "gmf"
import "testing"
import __regexp__ "regexp"

var tests = []testing.InternalTest{
	{"gmf.TestMultiplexer", gmf.TestMultiplexer},
}
var benchmarks = []testing.InternalBenchmark{}

func main() {
	testing.Main(__regexp__.MatchString, tests)
	testing.RunBenchmarks(__regexp__.MatchString, benchmarks)
}
