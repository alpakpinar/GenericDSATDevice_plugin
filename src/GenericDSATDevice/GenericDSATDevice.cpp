#include "GenericDSATDevice/GenericDSATDevice.hh"

using namespace BUTool;

GenericDSATDevice::GenericDSATDevice(std::vector<std::string> args) 
    : CommandList<GenericDSATDevice>("GenericDSAT"),
      GenericDSATHolder(args),
      RegisterHelper(std::static_pointer_cast<RegisterHelperIO>(genericDSATPtr), 
        BUTool::CommandListBase::TextIO)
{
    // Register device-specific commands to BUTool
    AddCommands();
}

GenericDSATDevice::~GenericDSATDevice() {
}

void GenericDSATDevice::AddCommands() {
    /*
     * Register commands for this device to BUTool interface.
     * "read" and "write" commands are inherited from BUTool::RegisterHelper,
     * and the "status" command is implemented via StatusDisplay function in this class.
     */
    AddCommand("read",&GenericDSATDevice::Read,
	     "Read from GenericDSAT\n"          \
	     "Usage: \n"                     \
	     "  read addr <count> <FLAGS>\n" \
	     "Flags: \n"                     \
	     "  D:  64bit words\n"           \
	     "  N:  suppress zero words\n",
	     &GenericDSATDevice::RegisterAutoComplete);
    AddCommandAlias("r","read");

    AddCommand("write",&GenericDSATDevice::Write,
	     "Write to GenericDSAT\n"           \
	     "Usage: \n"                        \
	     "  write addr <data> <count> \n",
	     &GenericDSATDevice::RegisterAutoComplete);
    AddCommandAlias("w","write");
    
    AddCommand("status",&GenericDSATDevice::StatusDisplay,
	     "Display status tables \n"  \
	     "Usage: \n"                          \
	     "  status level <table name>\n");
}

CommandReturn::status GenericDSATDevice::StatusDisplay(std::vector<std::string> strArg, std::vector<uint64_t> intArg) {
    std::string tableName("");
    int statusLevel = 1;
    
    // Determine what to do based on the number of arguments given
    switch (strArg.size()) {
        case 0:
            break;
        default:
        case 2:
            tableName = strArg[1];
            // Fall through
        case 1:
            // First argument must be a digit corresponding to the status level
            if (!isdigit(strArg[0][0])) {
                return CommandReturn::BAD_ARGS;
            }
            if ((intArg[0] < 1) || (intArg[0] > 9)) {
                return CommandReturn::BAD_ARGS;    
            }
            statusLevel = intArg[0];
            break;
    }
    
    // Call the StatusDisplay functionality of the underlying class
    std::ostringstream oss;
    genericDSATPtr->GenerateStatusDisplay(statusLevel, oss, tableName);
    
    // Print the resulting output stream and return
    BUTool::CommandListBase::TextIO->Print(Level::INFO, "%s", oss.str().c_str());
    return CommandReturn::OK;
    
}