#include "GenericDSAT/GenericDSAT.hh"

GenericDSAT::GenericDSAT(std::string addrTablePath) 
    : RegisterHelperIO(),
      statusDisplay((BUTool::RegisterHelperIO*)this)
{
    /*
     * Allocate an AddressTable with the given path to get registers and their addresses.
     * Also set up the array and the StatusDisplay object.
     */
    addressTable = new AddressTable(addrTablePath);

    // Using the API from DSAT, figure out the maximum address in the address table
    // We'll allocate an array of zeros with that many elements
    size_t maxAddress = addressTable->GetMaxAddress();
    std::cout << "Allocating an array with maximum index: " << maxAddress << std::endl;

    values.resize(maxAddress);
    std::fill(values.begin(), values.end(), 0);
}

GenericDSAT::~GenericDSAT() {
    /*
     * De-allocate the memory for the AddressTable instance.
     */
    if (addressTable != NULL) {
        delete addressTable;
    }
}

std::vector<std::string> GenericDSAT::GetRegsRegex(std::string regex) {
    /*
     * Returns an array of register names which match the given regular expression.
     */
    return addressTable->GetNames(regex);
}

uint32_t GenericDSAT::ReadAddress(uint32_t addr) {
    /*
     * Reads a 32-bit unsigned integer from the given address, via an array lookup.
     * Will throw an INVALID_ADDRESS exception if the address is outside of the array bounds.
     */
    if (addr > values.size()) {
        BUException::INVALID_ADDRESS e;
        e.Append("Address value out of range: ");

        char addressStr[10];
        sprintf(addressStr, "%u", addr);

        e.Append(addressStr);
        e.Append("\n");
        throw e;
    }
    return values[addr];
}

uint32_t GenericDSAT::ReadRegister(std::string const & reg) {
    /*
     * Reads a register with a given name. Uses the DSAT library
     * to figure out the address for a register.
     */
    uint16_t address = addressTable->GetItem(reg)->address;
    return ReadAddress(address);
}

void GenericDSAT::WriteAddress(uint32_t addr, uint32_t data) {
    /*
     * Writes a 32-bit unsigned integer to the specified address.
     * Will throw an INVALID_ADDRESS exception if the address is outside of the array bounds.
     */
    if (addr > values.size()) {
        BUException::INVALID_ADDRESS e;
        e.Append("Address value out of range: ");

        char addressStr[10];
        sprintf(addressStr, "%u", addr);

        e.Append(addressStr);
        e.Append("\n");
        throw e;
    }
    values[addr] = data;
}

void GenericDSAT::WriteRegister(std::string const & reg, uint32_t data) {
    /*
     * Writes a 32-bit unsigned integer to the specified register (by name).
     * Uses the DSAT library to figure out the address for a register.
     */
    uint16_t address = addressTable->GetItem(reg)->address;
    WriteAddress(address, data);
}

void GenericDSAT::WriteAction(std::string const & reg) {
    /*
     * Write a dummy value (0) to the given register.
     */
    uint32_t dummyValue = 0;
    uint16_t address = addressTable->GetItem(reg)->address;
    WriteAddress(address, dummyValue);
}

/*
 * Getter methods for the register specified via it's name
 */
uint32_t GenericDSAT::GetRegAddress(std::string const & reg) {
    return addressTable->GetItem(reg)->address;
}

uint32_t GenericDSAT::GetRegMask(std::string const & reg) {
    return addressTable->GetItem(reg)->mask;
}

uint32_t GenericDSAT::GetRegSize(std::string const & reg) {
    uint32_t mask = GetRegMask(reg);
    
    // To get the size of the register, shift the bit-values to right
    // until we get a 1 in the least significant place.
    while ((mask & 0x1) == 0) {
        mask = mask >> 1;
    }

    // Now, determine the size by counting number of 1-bits
    uint32_t registerSize = 0;
    while ((mask & 0x1) == 1) {
        mask = mask >> 1;
        registerSize += 1;
    } 
    
    return registerSize;
}

std::string GenericDSAT::GetRegMode(std::string const & reg) {
    uint8_t mode = addressTable->GetItem(reg)->mode;

    // Mode is bitwise, by looking at the individual bits, figure out the permissions
    // Empty: 0x0, Read: 0x1, Write: 0x2, Action: 0x4
    std::string permission = "";

    std::unordered_map<std::string, int> permissionMap;
    permissionMap["r"] = 1;
    permissionMap["w"] = 2;
    permissionMap["a"] = 4;

    for (auto item : permissionMap) {
        if ((mode & item.second) == 1) {
            permission += item.first;
        }
    }
    return permission;
}

std::string GenericDSAT::GetRegPermissions(std::string const & reg) {
    return GetRegMode(reg);
}

std::string GenericDSAT::GetRegDescription(std::string const & reg) {
    strStrMap userParams = addressTable->GetItem(reg)->user;
    if (userParams.find("description") != userParams.end()) {
        return userParams["description"];
    }
    return "";
}

std::string GenericDSAT::GetRegParameterValue(std::string const & reg, std::string const & name) {
    strStrMap userParams = addressTable->GetItem(reg)->user;
    auto iterator = userParams.find(name);

    // Handle the case where we can't find such a parameter
    if (iterator == userParams.end()) {
        BUException::BAD_VALUE e;
        e.Append("Register: ");
        e.Append(reg);
        e.Append(" Parameter not found: ");
        e.Append(name);
        e.Append("\n");
        throw e;
    }
    return iterator->second;
}

const strStrMap & GenericDSAT::GetRegParameters(std::string const & reg) {
    return addressTable->GetItem(reg)->user;
}

void GenericDSAT::GenerateStatusDisplay(size_t level,
           std::ostream & stream = std::cout,
           std::string const & singleTable = std::string("")) {

    /*
     * The function to be called when "status" is executed from the BUTool CLI.
     *
     * Using an instance of the BUTool::StatusDisplay object, the function will print
     * status tables as defined by the address table (registered in the constructor).
     */
    statusDisplay.Clear();
    statusDisplay.Report(level,stream,singleTable);
    statusDisplay.Clear();
}