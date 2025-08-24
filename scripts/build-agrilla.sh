#! /bin/bash
#------------------------------------------------------------------------------
# Script to build agrilla executable. If option -p is included the script
# will also create a .deb installation package
# This script MUST BE RUN from <root>/scripts/ folder
#
# usage:
#   cd scripts
#   ./build-agrilla.sh [-p]
#
# After running the script you will find the executable and a .deb installation
# package in folder <root>/z_build-area
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Display the help message
function DisplayHelp()
{
    echo "Usage: ./bulid-agrilla.sh [option]*"
    echo ""
    echo "Options:"
    echo "    -h --help        Print this help text."
    echo "    -p --package     Generate a .deb package."
    echo ""
}

#------------------------------------------------------------------------------
# main line starts here

E_SUCCESS=0         # success
E_NOARGS=65         # no arguments
E_BADPATH=66        # not running from <root>/scripts
E_BADARGS=67        # bad arguments
E_BUIL_ERROR=68

enhanced="\e[7m"
reset="\e[0m"

#get current directory and check we are running from <root>/scripts.
#For this I just check that "src" folder exists
scripts_path="${PWD}"
root_path=$(dirname "${PWD}")
if [[ ! -e "${root_path}/src" ]]; then
    echo "Error: not running from <root>/scripts"
    exit $E_BADPATH
fi

#parse command line parameters
# See: https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash
#
fCreatePackage=0
while [[ $# -gt 0 ]]
do
    key="$1"

    case $key in
        -h|--help)
        DisplayHelp
        exit 1
        ;;
        -p|--package)
        fCreatePackage=1
        shift       # past argument
        shift       # past value
        ;;
        *) # unknown option 
        DisplayHelp
        exit 1
        ;;
    esac
done

#path for building
build_path="${root_path}/z_build-area"
sources="${root_path}"

#create or clear build folder
if [[ ! -e ${build_path} ]]; then
    #create build folder
    echo -e "${enhanced}Creating build folder${reset}"
    mkdir ${build_path}
    echo "-- Build folder created"
    echo ""
elif [[ ! -d ${build_path} ]]; then
    #path exists but it is not a folder
    echo "Folder for building (${build_path}) already exists but is not a directory!"
    echo "Build aborted"
    exit $E_BUIL_ERROR
else
    # clear build folder
    echo -e "${enhanced}Removing last build${reset}"
    cd "${build_path}" || exit $E_BADPATH
    rm * -rf
    echo "-- Build folder now empty"
    echo ""
fi

# create makefile
cd "${build_path}"
echo -e "${enhanced}Creating makefile${reset}"
cmake -G "Unix Makefiles" ${sources}  || exit 1
echo ""

#build program
num_jobs=`getconf _NPROCESSORS_ONLN`	#number of jobs to create (as many as the number of processors)
echo -e "${enhanced}Building AGrilla. This will use ${num_jobs} jobs.${reset}"
start_time=$(date -u +"%s")
make -j$num_jobs || exit 1
end_time=$(date -u +"%s")
secs=$(($end_time-$start_time))
echo "Build time: $(($secs / 60))m:$(($secs % 60))s"

#prepare package:
if [[ "$fCreatePackage" -eq 1 ]]; then
    echo -e "${enhanced}Creating package${reset}"
    cd "${build_path}" || exit $E_BADPATH
    make package || exit 1
    echo "-- Done"
fi

cd "${scripts_path}" || exit $E_BADPATH
#exit $E_SUCCESS

