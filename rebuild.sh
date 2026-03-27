while getopts vs opt; do
    case $opt in
        v) opt_v=true ;;
        s) opt_s=true ;;
        *) echo 'error in command line parsing' >&2
           exit 1
    esac
done

if [ -f "CMakeCache.txt" ]; then
    echo "CMakeCache.txt exists rming"
    rm CMakeCache.txt
else
    echo "CMakeCache.txt does not exist"
fi

if [ -d "CMakeFiles" ]; then
    echo "CMakeFiles exists rming"
    rm -r CMakeFiles
else
    echo "CMakeFiles does not exist"
fi

cmake -S "./" -G "Unix Makefiles"

# option -s: skip to not actually build
if [ $opt_s ]; then
    exit
fi

# option -v: do verbose build
if [ $opt_v ]; then
    cmake --build . --verbose -j8
else
    cmake --build . -j8
fi

