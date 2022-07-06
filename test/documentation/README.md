## BUTool StatusDisplay Testing via DSAT Plugin

Using the address table located in `test/address_tables/test.adt`, a DSAT-device can be used
to test the `StatusDisplay` functionality of `BUTool`. This can be achieved by running the `run_BUTool_test.sh` script located in the root directory of this project. The sub-section below describes how this script works and what functionality is tested.

### Running BUTool Tests

**Note:** Make sure to source the `env.sh` before executing the testing script, so that the `BUTOOL_PATH` is correctly set in your environment.

`run_BUTool_test.sh` script will launch a `BUTool` session and load in the `GenericDSATDevice` class, and finally execute the commands specified in `test/scripts/BUTool_test.txt`. The output of the test will be the output of the `status` command, where each table tests a different functionality of `StatusDisplay`. These are listed below:

- `TEST_1`: Tests if the default hex format is correctly applied, such as the zero padding (on values larger than 10), the determination of column width and so on.
- `TEST_2`: Tests if the signed and unsigned integers are correctly displayed, given the written values to them (as by the instructions in `BUTool_test.txt`)
- `TEST_3`: Tests different status levels, the `STATUS_3` column must only appear when `status N` is executed with `N` greater or equal to 3.
- `TEST_4`: Tests the functionality to show a register only for non-zero values (`Show="nz"`), or for zero values (`Show="z"`). `Show="nzr"` attribute is also tested, where the entire row is suppressed if the value in such a register is zero.
- `TEST_5`: Tests enumerations by writing different values to those registers.
- `TEST_6`: Tests the floating point numbers computed by the `Format="m_..."` customized format.
- `TEST_7`: Tests regular 16-bit floating point values, special values like positive/negative infinity are checked here.
- `TEST_8`: Tests the resulting 64-bit values, from the merging of two 32-bit registers. One of those merged registers **must** have a `_LO` suffix at the end, and the other one must have a `_HI`.