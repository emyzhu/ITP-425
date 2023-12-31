#!/bin/bash
# Cmake into build directory
curl -O http://chalonverse.com/435/pa6.tar.gz
tar xzf pa6.tar.gz
echo "Compiling..."
mkdir build
cd build
RELEASE=ON CC=clang CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. || exit 1
build_failed=0
make 2> >(tee diagnostics.txt >&2)|| { echo "::error::Code did not compile!"; build_failed=1; }
cd ..
./diagnostics-json.py
if [[ "$build_failed" == 1 ]] ; then
	exit 1
fi

cd build
# Run clang-tidy
echo "Running clang-tidy..."
../run-clang-tidy.py -quiet -header-filter=".*/src/[a-zA-Z].*" -export-fixes=tidy.yaml || { echo "::warning::Unable to run clang-tidy"; }
# Return to root folder (so cwd is correct)
cd ..
./tidy-json.py

# Run student tests
echo "Running student tests..."
timeout 30 build/tests/tests [student]

# Run graded tests
echo "Running graded tests..."
BUILDFAILED=0
timeout 10 build/tests/tests [graded1] -r=github || BUILDFAILED=1
timeout 10 build/tests/tests [graded2] -r=github || BUILDFAILED=1
timeout 10 build/tests/tests [graded3] -r=github || BUILDFAILED=1
timeout 10 build/tests/tests [graded4] -r=github || BUILDFAILED=1

if (( $BUILDFAILED == 1 )); then
	echo "::error::Not all graded tests passed!"; exit 1;
fi
# Now try in address sanitizer...
#- mkdir buildAsan
#- cd buildAsan
#- CC=clang-4.0 CXX=clang++-4.0 SANITIZE=ON cmake ..
#- make
#- cd ..
#- echo "Running address sanitizer..."
#- timeout 10 buildAsan/tests/tests [graded]
