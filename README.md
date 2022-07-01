## Generic DSAT-based Device Plugin

This repository has the source code for a simple, generic [DSAT](https://gitlab.com/BU-EDF/dsat) based device plugin. This device
is capable of performing DSAT-based reads and writes, using an array allocated at runtime to hold the register values. It can be
registered to [BUTool](https://github.com/BU-Tools/BUTool/tree/release/v2.0) at runtime via it's device library, and supports the
`read`, `write` and `status` commands from BUTool CLI.

Some address tables are defined under the directory `test/address_tables`. Check out [DSAT](https://gitlab.com/BU-EDF/dsat) for
more examples of address tables in DSAT-format.

### Building the Code

**Note:** Since this plugin has dependencies to `BUTool`, these instructions assume that `BUTool` is built and installed under the path `/path/to/BUTool`. 

For building the project, the following steps must be taken:

- Clone the repository recursively, together with the `DSAT` sub-module
- Build the `DSAT` submodule
- Source the `env.sh` script with the correct `BUTOOL_PATH` and make the DSAT-device plugin

The following commands illustrate the process:

```bash
# Note: If you don't have SSH keys set up, you might want to use a different GitHub Auth 
git clone --recursive git@github.com:alpakpinar/GenericDSATDevice_plugin.git

# Build the DSAT sub-module
cd dsat/
make

# Build the plugin
cd ../
source env.sh /path/to/BUTool
make
```

Before building, make sure that `BUTOOL_PATH` environment variable is properly set to your BUTool install path, `/path/to/BUTool` after sourcing the `env.sh` script. To remove all build products, you can execute `make clean`.

### Running the Code with BUTool

Once the plugin is built using the instructions from the last sub-section, you should have the `libDSAT_GenericDSATDevice.so` library under the directory `lib`. This shared library file will be used to register the `GenericDSATDevice` to `BUTool`. 

The following commands illustrate this process:

```bash
cd /path/to/BUTool

# Set LD_LIBRARY_PATH correctly
source env.sh

# Start BUTool and add the DSATDevice library
/path/to/BUTool.exe -l /path/to/libDSAT_GenericDSATDevice.so
Registered device: GENERICDSAT
> add_device GENERICDSAT /path/to/address_table.adt
```

And you're done! You can do reads and writes, or display status tables via `status` command in the BUTool CLI.

