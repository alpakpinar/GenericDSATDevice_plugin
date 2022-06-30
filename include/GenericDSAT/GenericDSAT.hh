#ifndef __GENERIC_DSAT_HH__
#define __GENERIC_DSAT_HH__

// Includes from BUTool
#include "RegisterHelper/RegisterHelperIO.hh"
#include "StatusDisplay/StatusDisplay.hh"
#include "BUTool/ToolException.hh"

// Includes from DSAT sub-module
#include "AddressTable.hh"

#include "GenericDSAT/Exceptions.hh"

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdint.h>

// Number of registers in the address table, hard code it for now.
#define N_REGISTERS 1000

/*
 * Simple class that supports reads and writes on DSAT-based address tables.
 * Stores registers in an array of 32-bit unsigned integers.
 */
class GenericDSAT : public BUTool::RegisterHelperIO {

    public:
        GenericDSAT(std::string addrTablePath);
        ~GenericDSAT();

        typedef std::unordered_map<std::string, std::string> uMap;

        // Register name search
        std::vector<std::string> GetRegsRegex(std::string regex);

        // Reads by array index and register name
        uint32_t    ReadAddress (uint32_t addr);
        uint32_t    ReadRegister(std::string const & reg);

        // Writes by array index and register name
        void        WriteAddress (uint32_t addr, uint32_t data);
        void        WriteRegister(std::string const & reg, uint32_t data);

        // Action writes
        void        WriteAction(std::string const & reg);

        // Lookups by name
        uint32_t    GetRegAddress     (std::string const & reg);
        uint32_t    GetRegMask        (std::string const & reg);
        uint32_t    GetRegSize        (std::string const & reg);
        std::string GetRegMode        (std::string const & reg);
        std::string GetRegPermissions (std::string const & reg);
        std::string GetRegDescription (std::string const & reg);

        std::string   GetRegParameterValue(std::string const & reg, std::string const & name);
        const uMap &  GetRegParameters    (std::string const & reg);

        void GenerateStatusDisplay(size_t level,
            std::ostream & stream,
            std::string const & singleTable);

    private:
        // Member data
        uint32_t values[N_REGISTERS];
        AddressTable* addressTable;
    
        // StatusDisplay class instance to use to interact with BUTool's status command
        BUTool::StatusDisplay statusDisplay;

};

#endif