// clang-format off
#pragma once

#include <type_traits>
#include "cocos/bindings/jswrapper/SeApi.h"
#include "cocos/bindings/manual/jsb_conversions.h"
#include "../../cocos-test-gfx/tests/ScriptTest/Boids.h"

bool register_all_boids(se::Object *obj);                   // NOLINT

JSB_REGISTER_OBJECT_TYPE(application::BoidsOptions);
JSB_REGISTER_OBJECT_TYPE(application::BoidsManager);


extern se::Object *__jsb_application_BoidsOptions_proto; // NOLINT
extern se::Class * __jsb_application_BoidsOptions_class; // NOLINT

bool js_register_application_BoidsOptions(se::Object *obj); // NOLINT

template <>
bool sevalue_to_native(const se::Value &, application::BoidsOptions *, se::Object *ctx); //NOLINT

extern se::Object *__jsb_application_BoidsManager_proto; // NOLINT
extern se::Class * __jsb_application_BoidsManager_class; // NOLINT

bool js_register_application_BoidsManager(se::Object *obj); // NOLINT

SE_DECLARE_FUNC(js_boids_BoidsManager_destroy);
SE_DECLARE_FUNC(js_boids_BoidsManager_init);
SE_DECLARE_FUNC(js_boids_BoidsManager_tick);
// clang-format on
