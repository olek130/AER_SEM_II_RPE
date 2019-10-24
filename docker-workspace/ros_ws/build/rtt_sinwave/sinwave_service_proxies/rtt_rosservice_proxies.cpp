
#include <rtt/RTT.hpp>
#include <rtt/plugin/ServicePlugin.hpp>
#include <rtt/internal/GlobalService.hpp>

#include <rtt_roscomm/rtt_rosservice_registry_service.h>
#include <rtt_roscomm/rtt_rosservice_proxy.h>

////////////////////////////////////////////////////////////////////////////////
#include <sinwave/AddTwoDataPoints.h>

////////////////////////////////////////////////////////////////////////////////

namespace rtt_sinwave_ros_service_proxies { 

  bool registerROSServiceProxies() {
    // Get the ros service registry service
    ROSServiceRegistryServicePtr rosservice_registry = ROSServiceRegistryService::Instance();
    if(!rosservice_registry) {
      RTT::log(RTT::Error) << "Could not get an instance of the ROSServiceRegistryService! Not registering service proxies for sinwave" << RTT::endlog();
      return false;
    }

    // Get the factory registration operation
    RTT::OperationCaller<bool(ROSServiceProxyFactoryBase*)> register_service_factory = 
      rosservice_registry->getOperation("registerServiceFactory");

    // Make sure the registration operation is ready
    if(!register_service_factory.ready()) {
      RTT::log(RTT::Error) << "The ROSServiceRegistryService isn't ready! Not registering service proxies for sinwave" << RTT::endlog();
      return false;
    }

    // True at the end if all factories have been registered
    bool success = true;

    //////////////////////////////////////////////////////////////////////////////
    // Example: success = success && register_service_factory(new ROSServiceProxyFactory<std_srvs::Empty>("std_srvs/Empty"));
    success = success && register_service_factory(new ROSServiceProxyFactory<sinwave::AddTwoDataPoints>("sinwave/AddTwoDataPoints"));

    //////////////////////////////////////////////////////////////////////////////

    return success;
  }

}

extern "C" {
  bool loadRTTPlugin(RTT::TaskContext* c) { return rtt_sinwave_ros_service_proxies::registerROSServiceProxies(); }
  std::string getRTTPluginName () { return "rtt_sinwave_ros_service_proxies"; }
  std::string getRTTTargetName () { return OROCOS_TARGET_NAME; }
}
