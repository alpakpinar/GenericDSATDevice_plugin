#ifndef __GENERIC_DSAT_DEVICE_HH__
#define __GENERIC_DSAT_DEVICE_HH__

#include "BUTool/CommandList.hh"
#include "BUTool/DeviceFactory.hh"
#include "BUTextIO/BUTextIO.hh"

#include "RegisterHelper/RegisterHelper.hh"

#include "GenericDSAT/GenericDSAT.hh"

namespace BUTool {
    class GenericDSATDevice: public CommandList<GenericDSATDevice>,
                public RegisterHelper {

        public:
            GenericDSATDevice(std::vector<std::string> args);
            ~GenericDSATDevice();
        
        private:
            // Pointer to GenericDSAT instance for underlying operations
            GenericDSAT* genericDSATPtr;
            BUTextIO* textIO;

            // Function to register commands for the GenericDSATDevice class
            void AddCommands();

            CommandReturn::status StatusDisplay(std::vector<std::string>,std::vector<uint64_t>);

    };

    // Register GenericDSATDevice with the DeviceFactory  
    RegisterDevice(GenericDSATDevice,
            "GenericDSAT",
            "file/SM_SN",
            "a",
            "GenericDSAT",
            "Connection file for creating a DSAT-based device"
            );

}

#endif
