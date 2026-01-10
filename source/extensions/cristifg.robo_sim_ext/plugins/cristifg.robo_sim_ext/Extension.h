#include <carb/PluginUtils.h>
#include <carb/events/IEvents.h>
#include <carb/eventdispatcher/IEventDispatcher.h>
#include <carb/events/EventsUtils.h>
#include <carb/Interface.h>
#include <omni/ext/IExt.h>
#include <cstdint>

#define EXTENSION_NAME "cristifg.robo_sim_test.plugin"

using namespace carb;

const struct carb::PluginImplDesc kPluginImpl = {
    EXTENSION_NAME,
    "Robo Simulation Extension",
    "cristifg",
    carb::PluginHotReload::eDisabled,
    "dev"
};

CARB_PLUGIN_IMPL_DEPS(carb::logging::ILogging)

namespace cristifg {
namespace robo_sim_test {

    struct Extension : public omni::ext::IExt {
        void onStartup(const char* extId) override;
        void onShutdown() override;
    private:
        carb::eventdispatcher::ObserverGuard        m_subscription;
        int32_t                                     m_counter;

        carb::events::IEventStreamPtr               m_updateStream;
        carb::events::ISubscriptionPtr              m_updateSubscription;

        void onGlobalUpdate(const carb::eventdispatcher::Event&);
        void onUpdate(const carb::events::IEvent* );

    };
}
}

CARB_PLUGIN_IMPL(kPluginImpl, cristifg::robo_sim_test::Extension)

void fillInterface(cristifg::robo_sim_test::Extension& iface) {

}
