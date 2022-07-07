#!/bin/bash

# Path to BUTool must be specified as an environment variable (by sourcing env.sh) 
if [ -z ${BUTOOL_PATH} ]; then
  echo "BUTOOL_PATH environment variable is not defined, did you source env.sh?"
  echo "Exiting..."
  exit 1
fi

LIB_DSAT_DEVICE=$(realpath "lib/libDSAT_GenericDSATDevice.so")
ADDRESS_TABLE_PATH=$(realpath "test/address_tables/test.adt")

# Perform necessary text substitutions and save into a new txt file
# under a temporary directory, which we'll remove after the test
TMP_DIR=$(realpath "./tmp")
if [ -d ${TMP_DIR} ]; then
  rm -r ${TMP_DIR}
fi

mkdir ${TMP_DIR}
ORIGINAL_SCRIPT_FILE="test/scripts/BUTool_test.txt"
OUTPUT_FILE="${TMP_DIR}/log_BUTool.txt"
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

pushd ${BUTOOL_PATH} > /dev/null

source env.sh
BUTOOL_EXEC="${BUTOOL_PATH}/bin/BUTool/BUTool.exe"

# Execute
${BUTOOL_EXEC} -X ${SCRIPT_FILE}

popd > /dev/null

# Remove unnecessary lines from the log file 
sed -i '/^Write to register/d' ${OUTPUT_FILE} 

# Now, compare the content of this file with the golden output from StatusDisplay
GOLDEN_LOG_FILE=$(realpath "./test/golden/log_BUTool_golden.txt")

BOLD=$(tput bold)
NORMAL=$(tput sgr0)

DIFF=$(diff ${GOLDEN_LOG_FILE} ${OUTPUT_FILE})
if [ "${DIFF}" != "" ]; then
  echo -e "\xE2\x9D\x8C ${BOLD}StatusDisplay output does not match with golden logs!${NORMAL}"
  echo "The diff:"
  echo ${DIFF}
  rm -r ${TMP_DIR}
  exit 1
fi

echo -e "\n\xE2\x9C\x94 ${BOLD}StatusDisplay output matches with the golden log${NORMAL}"
echo -e "\nCleaning up.."
echo -e "\nDone: $(date)"

# Remove the temporary directory after the job is done
rm -r ${TMP_DIR}


