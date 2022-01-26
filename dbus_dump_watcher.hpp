#pragma once

#include "dump_utility.hpp"

#include <map>
#include <memory>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/bus/match.hpp>

namespace openpower::dump
{
using ::openpower::dump::utility::DumpType;
using ::openpower::dump::utility::ManagedObjectType;
class DBusDumpWatcher
{
  public:
    DBusDumpWatcher() = delete;
    DBusDumpWatcher(const DBusDumpWatcher&) = delete;
    DBusDumpWatcher& operator=(const DBusDumpWatcher&) = delete;
    DBusDumpWatcher(DBusDumpWatcher&&) = delete;
    DBusDumpWatcher& operator=(DBusDumpWatcher&&) = delete;
    virtual ~DBusDumpWatcher() = default;

    /**
     * @brief Watch on new dump objects created and property change
     * @param[in] bus - Bus to attach to.
     * @param[in] entryIntf - dump entry interface (BMC/Host/SBE/Hardware)
     * @param[in] dumpType - dump type to watch
     */
    DBusDumpWatcher(sdbusplus::bus::bus& bus, const std::string& entryIntf,
                    DumpType dumpType);

    /**
     * @brief Add all in progress dumps to property watch
     * @param[in] objects dump objects whose progress is not yet completed
     * @return void
     */
    void addInProgressDumpsToWatch(ManagedObjectType&& objects);

  private:
    /**
     * @brief Callback method for creation of dump entry object
     * @param[in] msg response msg from D-bus request
     * @return void
     */
    void interfaceAdded(sdbusplus::message::message& msg);

    /**
     * @brief Callback method for deletion of dump entry object
     * @param[in] msg response msg from D-bus request
     * @return void
     */
    void interfaceRemoved(sdbusplus::message::message& msg);

    /**
     * @brief Callback method for property change on the entry object
     * @param[in] objPath Object path of the dump entry
     * @param[in] id ID of the dump entry
     * @param[in] msg response msg from D-bus request
     * @return void
     */
    void propertiesChanged(const std::string& objPath, uint32_t id,
                           sdbusplus::message::message& msg);

    /** @brief D-Bus to connect to */
    sdbusplus::bus::bus& _bus;

    /** @brief entry interface to watch for */
    std::string _entryIntf;

    /** @brief type of the dump to watch for */
    DumpType _dumpType;

    /** @brief watch pointer for interfaces added */
    std::unique_ptr<sdbusplus::bus::match_t> _intfAddWatch;

    /** @brief watch pointer for interfaces removed */
    std::unique_ptr<sdbusplus::bus::match_t> _intfRemWatch;

    /** @brief map of property change request for the corresponding entry */
    std::map<std::string, std::unique_ptr<sdbusplus::bus::match_t>>
        _entryPropWatchList;
};
} // namespace openpower::dump
