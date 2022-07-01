main () {
    # Path to BUTool must be specified from the command line
    if [ $# -eq 0 ]; then
        echo "Please provide the path to BUTool via command line, exiting."
        return
    fi

    # Set the BUTOOL_PATH environment variable, to be used by the build
    export BUTOOL_PATH=${1}

    # Update the LD_LIBRARY_PATH
    DSAT_LIB_DIR=${PWD}/dsat/lib
    PLUGIN_LIB_DIR=${PWD}/lib

    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${DSAT_LIB_DIR}:${PLUGIN_LIB_DIR}
}

main
