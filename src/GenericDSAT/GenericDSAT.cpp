#include "GenericDSAT/GenericDSAT.hh"

GenericDSAT::GenericDSAT(std::string addrTablePath) 
    : RegisterHelperIO(),
      statusDisplay((BUTool::RegisterHelperIO*)this)
{
    /*
     * Allocate an AddressTable with the given path to get addresses and items.
     * Also set up the array and the StatusDisplay object.
     */
    addressTable = new AddressTable(addrTablePath);

    // Using the API from DSAT, figure out the maximum address in the address table
    // We'll allocate an array of zeros with that many elements via memset
    size_t maxAddress = addressTable->GetMaxAddress();
    std::cout << "Initializing an array of size: " << maxAddress << std::endl;
    memset(values, 0, maxAddress);
}

GenericDSAT::~GenericDSAT() {
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
    // Check that the address is within the array range
    size_t arraySize = sizeof(values) / sizeof(uint32_t);
    if (addr > arraySize) {
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
    // Retrieve the address for the register name, from the address table
    uint16_t address = addressTable->GetItem(reg)->address;
    return ReadAddress(address);
}

void GenericDSAT::WriteAddress(uint32_t addr, uint32_t data) {
    size_t arraySize = sizeof(values) / sizeof(uint32_t);
    if (addr > arraySize) {
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
    uint16_t address = addressTable->GetItem(reg)->address;
    values[address] = data;
}

void GenericDSAT::WriteAction(std::string const & reg) {
    /*
     * Write a dummy value (0) to the given register.
     */
    uint32_t dummyValue = 0;
    uint16_t address = addressTable->GetItem(reg)->address;
    values[address] = dummyValue;
}

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

    statusDisplay.Clear();
    statusDisplay.Report(level,stream,singleTable);
    statusDisplay.Clear();
}