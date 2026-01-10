#include "Extension.h"
#include <carb/PluginUtils.h>
#include <carb/logging/Log.h>
#include <carb/events/IEvents.h>
#include <omni/kit/IApp.h>
#include <iostream>

using namespace cristifg::robo_sim_test;


void Extension::onStartup(const char* extId) {
    CARB_LOG_INFO("ROBOSIM : ONSTARTUP");

    auto ed = carb::getCachedInterface<carb::eventdispatcher::IEventDispatcher>();

    m_subscription = ed->observeEvent(
        carb::RStringKey("UpdateEventGlobal"),
        carb::eventdispatcher::kDefaultOrder,
        omni::kit::kGlobalEventUpdate,
        [this](const carb::eventdispatcher::Event& event) {

            if (m_counter % 100 == 0) {
                CARB_LOG_INFO(EXTENSION_NAME " : %d update passed \n", m_counter);
            }

            m_counter++;

            onGlobalUpdate(event);
        });
}

void Extension::onShutdown() {
    CARB_LOG_INFO("ROBOSIM : ONSHUTDOWN");
}

void Extension::onGlobalUpdate(const carb::eventdispatcher::Event& event) {
    CARB_LOG_INFO("ROBONOSIM : onGlobalUpdate");
}

void Extension::onUpdate(const carb::events::IEvent* ev) {
    CARB_LOG_INFO(EXTENSION_NAME ": ROBOSIM onUpdate");
}
