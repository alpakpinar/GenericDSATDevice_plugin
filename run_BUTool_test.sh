#!/bin/bash

# Path to BUTool must be specified as the first and only CL argument
if [ $# -eq 0 ]; then
  echo "Please specify the path to BUTool, exiting"
  exit 1
fi

BUTOOL_PATH=$(realpath ${1})

LIB_DSAT_DEVICE=$(realpath "lib/libDSAT_GenericDSATDevice.so")
ADDRESS_TABLE_PATH=$(realpath "test/address_tables/test.adt")

# Perform necessary text substitutions and save into a new txt file
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
