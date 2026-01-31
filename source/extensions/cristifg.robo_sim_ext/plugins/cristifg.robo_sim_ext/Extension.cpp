#include "Extension.h"
#include <carb/PluginUtils.h>
#include <carb/logging/Log.h>
#include <carb/events/IEvents.h>
#include <omni/kit/IApp.h>
#include <iostream>
#include <carb/InterfaceUtils.h>

#include <omni/usd/UsdContext.h>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usd/references.h>
#include <pxr/usd/usdPhysics/articulationRootAPI.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usdSkel/root.h>
#include <pxr/usd/usdSkel/bindingAPI.h>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usdSkel/root.h>
#include <pxr/usd/usdSkel/bindingAPI.h>
#include <pxr/usd/usdPhysics/articulationRootAPI.h>
#include <omni/usd/UsdContext.h>


using namespace cristifg::robo_sim_test;


static usdrt::UsdStageRefPtr getActiveStage()
    {
        const std::vector<PXR_NS::UsdStageRefPtr> allStages = PXR_NS::UsdUtilsStageCache::Get().GetAllStages();
        if (allStages.size() != 1) // only support one USD stage
            return nullptr;

        auto stage_id = PXR_NS::UsdUtilsStageCache::Get().GetId(allStages[0]).ToLongInt();
        return usdrt::UsdStage::Attach(omni::fabric::UsdStageId(stage_id));
    }


static void onStage(carb::events::IEvent* event) {

                if (event->type == (carb::events::EventType)omni::usd::StageEventType::eOpened) {
                    CARB_LOG_INFO("STAGE OPENED!!!");
                }
}


void Extension::onStartup(const char* extId) {
    CARB_LOG_INFO("ROBOSIM : ONSTARTUP");

    m_physx = carb::getCachedInterface<omni::physx::IPhysx>();

    auto uniqueAccelerometer = make_unique<AccelerometerSensor>("", m_physx);

    m_allAccelerometers.emplace("", make_unique<AccelerometerSensor>("", m_physx));

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


    m_subscriptionStage = ed->observeEvent(
        carb::RStringKey("cristifg.robo_sim_ext.StageOpenedObserver"),
        carb::eventdispatcher::kDefaultOrder,
        carb::RString("omni.usd::stage:opened"),
        [this](const carb::eventdispatcher::Event& event) {
            CARB_LOG_INFO("STAGE EVENT!!!");

        auto context = omni::usd::UsdContext::getContext("");

        if (context) {
            auto stageUrl = context->getStageUrl();

            CARB_LOG_INFO(EXTENSION_NAME " : STAGE URL %s ", stageUrl.c_str());
        }

        pxr::UsdStageRefPtr stage = context->getStage();
        if (stage) {
            CARB_LOG_INFO("FOUND STAGE!!!");
            pxr::SdfPath robotPath("/World/RoboSim1");
            auto robotPrim = stage->DefinePrim(robotPath, pxr::TfToken("Xform"));
            if (robotPrim) {
                CARB_LOG_INFO("ROBO PATH");
                robotPrim.GetReferences().AddReference("/home/cristianflorin//robo_3d/robo_righ.usdc");
                pxr::UsdPhysicsArticulationRootAPI::Apply(robotPrim);
                auto xform = pxr::UsdGeomXform(robotPrim);
                xform.AddTranslateOp().Set(pxr::GfVec3d(0.0, 0.0, 0.0));

                // Folosim UsdPrimRange cu o setare specială pentru a vedea în interiorul referințelor
                auto range = pxr::UsdPrimRange::Stage(stage, pxr::UsdTraverseInstanceProxies());

                for (auto it = range.begin(); it != range.end(); ++it) {
                pxr::UsdPrim prim = *it;

    // Logăm fiecare Prim găsit pentru debug (doar primele 10 să nu umplem consola)
    // CARB_LOG_INFO("Checking Prim: %s", prim.GetPath().GetText());

                    if (prim.IsA<pxr::UsdSkelRoot>()) {
                        CARB_LOG_INFO("!!!! [SUCCESS] FOUND SKELROOT AT: %s", prim.GetPath().GetText());
                    } else {
                        CARB_LOG_INFO("NOPE NOT YET!");
                    }
                }
                /*

                for (auto prim : pxr::UsdPrimRange(robotPrim)) {
                    if (prim.IsA<pxr::UsdSkelRoot>()) {
                        CARB_LOG_INFO(EXTENSION_NAME " : ROBO_SIM_1 HAS RIG!!!");

                        if (prim.HasAPI<pxr::UsdSkelBindingAPI>()) {
                            CARB_LOG_INFO(EXTENSION_NAME ": ROBO_SIM_1 HAS skinning present!");
                        }
                    }
                }

            */

            }
        }
        }
    );





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
