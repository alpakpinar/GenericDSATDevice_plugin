#ifndef __GENERIC_DSAT_DEVICE_HH__
#define __GENERIC_DSAT_DEVICE_HH__

#include "BUTool/CommandList.hh"
#include "BUTool/DeviceFactory.hh"
#include "BUTextIO/BUTextIO.hh"

#include "RegisterHelper/RegisterHelper.hh"

#include "GenericDSAT/GenericDSAT.hh"

namespace BUTool {
    /*
     * This class holds a smart pointer to GenericDSAT class instance
     * that is used by the inheriting class GenericDSATDevice
     */
    class GenericDSATHolder {
    public:
        GenericDSATHolder(std::vector<std::string> const & args) {
            genericDSATPtr = std::make_shared<GenericDSAT>(args[0]);
        }
    protected:
        std::shared_ptr<GenericDSAT> genericDSATPtr;
    private:
        GenericDSATHolder();
    };

    class GenericDSATDevice: public CommandList<GenericDSATDevice>,
            public GenericDSATHolder,
            public RegisterHelper {

        public:
            GenericDSATDevice(std::vector<std::string> args);
            ~GenericDSATDevice();
        
        private:
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
            )

}

#endif
