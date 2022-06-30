#include "GenericDSAT/GenericDSAT.hh"

GenericDSAT::GenericDSAT(std::string addrTablePath) {
    /*
     * Allocate an AddressTable with the given path to get addresses and items.
     * Also set up the array and the StatusDisplay object.
     */
    addressTable = new AddressTable(addrTablePath);

    statusDisplay = (BUTool::RegisterHelperIO*)this;

    // Get the list of register names and store every value in the array
    std::vector<std::string> registerNames = addressTable->GetNames("*");
    for (std::string name : registerNames) {
        Item const * item = addressTable->GetItem(name);

        /* Initialize the spot with a value of 0
         * Address of the register gives us the array index
         * First make sure that the address is within array bounds
         */
        if (item->address > N_REGISTERS) {
            BUException::INVALID_ADDRESS e;
            e.Append("Address given: ");
            e.Append(item->address.c_str());
            e.Append("\n");
            throw e;
        }
        values[item->address] = 0;
    }
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
    return values[addr];
}

uint32_t GenericDSAT::ReadRegister(std::string const & reg) {
    // Retrieve the address for the register name, from the address table
    uint16_t address = addressTable->GetItem(reg)->address;
    return ReadAddress(address);
}

void GenericDSAT::WriteAddress(uint32_t addr, uint32_t data) {
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
    return 0;
}

std::string GenericDSAT::GetRegMode(std::string const & reg) {
    uint8_t mode = addressTable->GetItem(reg)->mode;
    switch(mode) {
        case 0:
            return "r";
        case 1:
            return "w";
        case 2:
            return "a";
        case 3:
            return "i";
    }
}

std::string GenericDSAT::GetRegPermissions(std::string const & reg) {
    return GetRegMode(reg);
}

std::string GenericDSAT::GetRegDescription(std::string const & reg) {
    strStrMap userParams = addressTable->GetItem(reg)->user;
    if (userParams.find("description") != userParams.end()) {
        return userParams.find("description");
    }
    return "";
}

void GenericDSAT::GenerateStatusDisplay(size_t level,
           std::ostream & stream = std::cout,
           std::string const & singleTable = std::string("")) {

    statusDisplay.Clear();
    statusDisplay.Report(level,stream,singleTable);
    statusDisplay.Clear();
}