#!/bin/bash

# Path to BUTool must be specified as an environment variable (by sourcing env.sh) 
if [ -z ${BUTOOL_PATH} ]; then
  echo "Please specify the path to BUTool, exiting"
  exit 1
fi

LIB_DSAT_DEVICE=$(realpath "lib/libDSAT_GenericDSATDevice.so")
ADDRESS_TABLE_PATH=$(realpath "test/address_tables/test.adt")

# The output txt file to save BUTool output
LOG_DIR="./logs"
if [ ! -d ${LOG_DIR} ]; then
  mkdir -p ${LOG_DIR}
fi

TIMESTAMP="$(date +%d%b%y_%H%M%S)"
OUTPUT_FILE=$(realpath "${LOG_DIR}/log_BUTool_${TIMESTAMP}.txt")

# Perform necessary text substitutions and save into a new txt file
# under a temporary directory, which we'll remove after the test
TMP_DIR=$(realpath "./tmp")
if [ -d ${TMP_DIR} ]; then
  rm -r ${TMP_DIR}
fi

mkdir ${TMP_DIR}
ORIGINAL_SCRIPT_FILE="test/scripts/BUTool_test.txt"
SCRIPT_FILE="${TMP_DIR}/script.txt"
cp ${ORIGINAL_SCRIPT_FILE} ${SCRIPT_FILE}

sed -i "s|@LIB_DSAT_DEVICE|${LIB_DSAT_DEVICE}|g" ${SCRIPT_FILE}
sed -i "s|@ADDRESS_TABLE_PATH|${ADDRESS_TABLE_PATH}|g" ${SCRIPT_FILE}
sed -i "s|@OUTPUT_FILE|${OUTPUT_FILE}|g" ${SCRIPT_FILE}

# Remove comments and empty lines
sed -i "/^#/d" ${SCRIPT_FILE}
sed -i "/^$/d" ${SCRIPT_FILE}

# Set up environment and run BUTool
source env.sh ${BUTOOL_PATH}

pushd ${BUTOOL_PATH}

source env.sh
BUTOOL_EXEC="${BUTOOL_PATH}/bin/BUTool/BUTool.exe"

# Execute
${BUTOOL_EXEC} -X ${SCRIPT_FILE}

popd

rm -r ${TMP_DIR}
