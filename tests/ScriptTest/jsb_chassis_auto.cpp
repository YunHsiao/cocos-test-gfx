
// clang-format off
#include "../../cocos-test-gfx/tests/ScriptTest/jsb_chassis_auto.h"
#include "cocos/bindings/manual/jsb_conversions.h"
#include "cocos/bindings/manual/jsb_global.h"
#include "Chassis.h"

#ifndef JSB_ALLOC
#define JSB_ALLOC(kls, ...) new (std::nothrow) kls(__VA_ARGS__)
#endif

#ifndef JSB_FREE
#define JSB_FREE(ptr) delete ptr
#endif

#if CC_DEBUG
static bool js_chassis_getter_return_true(se::State& s) // NOLINT(readability-identifier-naming)
{
    s.rval().setBoolean(true);
    return true;
}
SE_BIND_PROP_GET(js_chassis_getter_return_true)
#endif
se::Object* __jsb_cc_experimental_TransformView_proto = nullptr; // NOLINT
se::Class* __jsb_cc_experimental_TransformView_class = nullptr;  // NOLINT

static bool js_chassis_TransformView_setParent(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::TransformView>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        HolderType<cc::experimental::TransformView*, false> arg0 = {};
        ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
        SE_PRECONDITION2(ok, false, "Error processing arguments");
        cobj->setParent(arg0.value());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_chassis_TransformView_setParent)

static bool js_chassis_TransformView_setPosition(se::State& s) // NOLINT(readability-identifier-naming)
{
    CC_UNUSED bool ok = true;
    auto* cobj = SE_THIS_OBJECT<cc::experimental::TransformView>(s);
    // SE_PRECONDITION2( cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    do {
        if (argc == 1) {
            HolderType<float*, false> arg0 = {};

            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            cobj->setPosition(arg0.value());
            return true;
        }
    } while(false);

    do {
        if (argc == 3) {
            HolderType<float, false> arg0 = {};
            HolderType<float, false> arg1 = {};
            HolderType<float, false> arg2 = {};

            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[1], &arg1, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[2], &arg2, s.thisObject());
            if (!ok) { ok = true; break; }
            cobj->setPosition(arg0.value(), arg1.value(), arg2.value());
            return true;
        }
    } while(false);

    SE_REPORT_ERROR("wrong number of arguments: %d", (int)argc);
    return false;
}
SE_BIND_FUNC(js_chassis_TransformView_setPosition)

static bool js_chassis_TransformView_setRotation(se::State& s) // NOLINT(readability-identifier-naming)
{
    CC_UNUSED bool ok = true;
    auto* cobj = SE_THIS_OBJECT<cc::experimental::TransformView>(s);
    // SE_PRECONDITION2( cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    do {
        if (argc == 1) {
            HolderType<float*, false> arg0 = {};

            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            cobj->setRotation(arg0.value());
            return true;
        }
    } while(false);

    do {
        if (argc == 4) {
            HolderType<float, false> arg0 = {};
            HolderType<float, false> arg1 = {};
            HolderType<float, false> arg2 = {};
            HolderType<float, false> arg3 = {};

            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[1], &arg1, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[2], &arg2, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[3], &arg3, s.thisObject());
            if (!ok) { ok = true; break; }
            cobj->setRotation(arg0.value(), arg1.value(), arg2.value(), arg3.value());
            return true;
        }
    } while(false);

    SE_REPORT_ERROR("wrong number of arguments: %d", (int)argc);
    return false;
}
SE_BIND_FUNC(js_chassis_TransformView_setRotation)

static bool js_chassis_TransformView_setRotationFromEuler(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::TransformView>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 3) {
        HolderType<float, false> arg0 = {};
        HolderType<float, false> arg1 = {};
        HolderType<float, false> arg2 = {};
        ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
        ok &= sevalue_to_native(args[1], &arg1, s.thisObject());
        ok &= sevalue_to_native(args[2], &arg2, s.thisObject());
        SE_PRECONDITION2(ok, false, "Error processing arguments");
        cobj->setRotationFromEuler(arg0.value(), arg1.value(), arg2.value());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 3);
    return false;
}
SE_BIND_FUNC(js_chassis_TransformView_setRotationFromEuler)

static bool js_chassis_TransformView_setScale(se::State& s) // NOLINT(readability-identifier-naming)
{
    CC_UNUSED bool ok = true;
    auto* cobj = SE_THIS_OBJECT<cc::experimental::TransformView>(s);
    // SE_PRECONDITION2( cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    do {
        if (argc == 1) {
            HolderType<float*, false> arg0 = {};

            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            cobj->setScale(arg0.value());
            return true;
        }
    } while(false);

    do {
        if (argc == 3) {
            HolderType<float, false> arg0 = {};
            HolderType<float, false> arg1 = {};
            HolderType<float, false> arg2 = {};

            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[1], &arg1, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[2], &arg2, s.thisObject());
            if (!ok) { ok = true; break; }
            cobj->setScale(arg0.value(), arg1.value(), arg2.value());
            return true;
        }
    } while(false);

    SE_REPORT_ERROR("wrong number of arguments: %d", (int)argc);
    return false;
}
SE_BIND_FUNC(js_chassis_TransformView_setScale)

static bool js_chassis_TransformView_updateWorldTransform(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::TransformView>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->updateWorldTransform();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_TransformView_updateWorldTransform)

SE_DECLARE_FINALIZE_FUNC(js_cc_experimental_TransformView_finalize)

static bool js_chassis_TransformView_constructor(se::State& s) // NOLINT(readability-identifier-naming) constructor_overloaded.c
{
    CC_UNUSED bool ok = true;
    const auto& args = s.args();
    size_t argc = args.size();
    do {
        if (argc == 1) {
            HolderType<int, false> arg0 = {};
            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            auto *ptr = JSB_MAKE_PRIVATE_OBJECT(cc::experimental::TransformView, arg0.value());
            s.thisObject()->setPrivateObject(ptr);
            return true;
        }
    } while(false);
    do {
        if (argc == 0) {
            auto *ptr = JSB_MAKE_PRIVATE_OBJECT(cc::experimental::TransformView);
            s.thisObject()->setPrivateObject(ptr);
            return true;
        }
    } while(false);
    SE_REPORT_ERROR("wrong number of arguments: %d", (int)argc);
    return false;
}
SE_BIND_CTOR(js_chassis_TransformView_constructor, __jsb_cc_experimental_TransformView_class, js_cc_experimental_TransformView_finalize)

static bool js_cc_experimental_TransformView_finalize(se::State& s) // NOLINT(readability-identifier-naming)
{
    return true;
}
SE_BIND_FINALIZE_FUNC(js_cc_experimental_TransformView_finalize)

bool js_register_chassis_TransformView(se::Object* obj) // NOLINT(readability-identifier-naming)
{
    auto* cls = se::Class::create("TransformView", obj, nullptr, _SE(js_chassis_TransformView_constructor));

#if CC_DEBUG
    cls->defineStaticProperty("isJSBClass", _SE(js_chassis_getter_return_true), nullptr);
#endif
    cls->defineFunction("setParent", _SE(js_chassis_TransformView_setParent));
    cls->defineFunction("setPosition", _SE(js_chassis_TransformView_setPosition));
    cls->defineFunction("setRotation", _SE(js_chassis_TransformView_setRotation));
    cls->defineFunction("setRotationFromEuler", _SE(js_chassis_TransformView_setRotationFromEuler));
    cls->defineFunction("setScale", _SE(js_chassis_TransformView_setScale));
    cls->defineFunction("updateWorldTransform", _SE(js_chassis_TransformView_updateWorldTransform));
    cls->defineFinalizeFunction(_SE(js_cc_experimental_TransformView_finalize));
    cls->install();
    JSBClassType::registerClass<cc::experimental::TransformView>(cls);

    __jsb_cc_experimental_TransformView_proto = cls->getProto();
    __jsb_cc_experimental_TransformView_class = cls;


    se::ScriptEngine::getInstance()->clearException();
    return true;
}
se::Object* __jsb_cc_experimental_ModelView_proto = nullptr; // NOLINT
se::Class* __jsb_cc_experimental_ModelView_class = nullptr;  // NOLINT

static bool js_chassis_ModelView_setColor(se::State& s) // NOLINT(readability-identifier-naming)
{
    CC_UNUSED bool ok = true;
    auto* cobj = SE_THIS_OBJECT<cc::experimental::ModelView>(s);
    // SE_PRECONDITION2( cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    do {
        if (argc == 1) {
            HolderType<float*, false> arg0 = {};

            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            cobj->setColor(arg0.value());
            return true;
        }
    } while(false);

    do {
        if (argc == 4) {
            HolderType<float, false> arg0 = {};
            HolderType<float, false> arg1 = {};
            HolderType<float, false> arg2 = {};
            HolderType<float, false> arg3 = {};

            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[1], &arg1, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[2], &arg2, s.thisObject());
            if (!ok) { ok = true; break; }
            ok &= sevalue_to_native(args[3], &arg3, s.thisObject());
            if (!ok) { ok = true; break; }
            cobj->setColor(arg0.value(), arg1.value(), arg2.value(), arg3.value());
            return true;
        }
    } while(false);

    SE_REPORT_ERROR("wrong number of arguments: %d", (int)argc);
    return false;
}
SE_BIND_FUNC(js_chassis_ModelView_setColor)

static bool js_chassis_ModelView_setEnabled(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::ModelView>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        HolderType<bool, false> arg0 = {};
        ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
        SE_PRECONDITION2(ok, false, "Error processing arguments");
        cobj->setEnabled(arg0.value());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_chassis_ModelView_setEnabled)

static bool js_chassis_ModelView_setTransform(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::ModelView>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 1) {
        HolderType<const cc::experimental::TransformView*, false> arg0 = {};
        ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
        SE_PRECONDITION2(ok, false, "Error processing arguments");
        cobj->setTransform(arg0.value());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 1);
    return false;
}
SE_BIND_FUNC(js_chassis_ModelView_setTransform)

SE_DECLARE_FINALIZE_FUNC(js_cc_experimental_ModelView_finalize)

static bool js_chassis_ModelView_constructor(se::State& s) // NOLINT(readability-identifier-naming) constructor_overloaded.c
{
    CC_UNUSED bool ok = true;
    const auto& args = s.args();
    size_t argc = args.size();
    do {
        if (argc == 1) {
            HolderType<int, false> arg0 = {};
            ok &= sevalue_to_native(args[0], &arg0, s.thisObject());
            if (!ok) { ok = true; break; }
            auto *ptr = JSB_MAKE_PRIVATE_OBJECT(cc::experimental::ModelView, arg0.value());
            s.thisObject()->setPrivateObject(ptr);
            return true;
        }
    } while(false);
    do {
        if (argc == 0) {
            auto *ptr = JSB_MAKE_PRIVATE_OBJECT(cc::experimental::ModelView);
            s.thisObject()->setPrivateObject(ptr);
            return true;
        }
    } while(false);
    SE_REPORT_ERROR("wrong number of arguments: %d", (int)argc);
    return false;
}
SE_BIND_CTOR(js_chassis_ModelView_constructor, __jsb_cc_experimental_ModelView_class, js_cc_experimental_ModelView_finalize)

static bool js_cc_experimental_ModelView_finalize(se::State& s) // NOLINT(readability-identifier-naming)
{
    return true;
}
SE_BIND_FINALIZE_FUNC(js_cc_experimental_ModelView_finalize)

bool js_register_chassis_ModelView(se::Object* obj) // NOLINT(readability-identifier-naming)
{
    auto* cls = se::Class::create("ModelView", obj, nullptr, _SE(js_chassis_ModelView_constructor));

#if CC_DEBUG
    cls->defineStaticProperty("isJSBClass", _SE(js_chassis_getter_return_true), nullptr);
#endif
    cls->defineFunction("setColor", _SE(js_chassis_ModelView_setColor));
    cls->defineFunction("setEnabled", _SE(js_chassis_ModelView_setEnabled));
    cls->defineFunction("setTransform", _SE(js_chassis_ModelView_setTransform));
    cls->defineFinalizeFunction(_SE(js_cc_experimental_ModelView_finalize));
    cls->install();
    JSBClassType::registerClass<cc::experimental::ModelView>(cls);

    __jsb_cc_experimental_ModelView_proto = cls->getProto();
    __jsb_cc_experimental_ModelView_class = cls;


    se::ScriptEngine::getInstance()->clearException();
    return true;
}
se::Object* __jsb_cc_experimental_Root_proto = nullptr; // NOLINT
se::Class* __jsb_cc_experimental_Root_class = nullptr;  // NOLINT

static bool js_chassis_Root_createModel(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::Root>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cc::experimental::ModelView* result = cobj->createModel();
        ok &= nativevalue_to_se(result, s.rval(), nullptr /*ctx*/);
        SE_PRECONDITION2(ok, false, "Error processing arguments");
        s.rval().toObject()->getPrivateObject()->tryAllowDestroyInGC();
        SE_HOLD_RETURN_VALUE(result, s.thisObject(), s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_Root_createModel)

static bool js_chassis_Root_createTransform(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::Root>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cc::experimental::TransformView* result = cobj->createTransform();
        ok &= nativevalue_to_se(result, s.rval(), nullptr /*ctx*/);
        SE_PRECONDITION2(ok, false, "Error processing arguments");
        s.rval().toObject()->getPrivateObject()->tryAllowDestroyInGC();
        SE_HOLD_RETURN_VALUE(result, s.thisObject(), s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_Root_createTransform)

static bool js_chassis_Root_destroy(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::Root>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->destroy();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_Root_destroy)

static bool js_chassis_Root_initialize(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::Root>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->initialize();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_Root_initialize)

static bool js_chassis_Root_render(se::State& s) // NOLINT(readability-identifier-naming)
{
    auto* cobj = SE_THIS_OBJECT<cc::experimental::Root>(s);
    // SE_PRECONDITION2(cobj, false, "Invalid Native Object");
    if (nullptr == cobj) return true;
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cobj->render();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_Root_render)

static bool js_chassis_Root_getInstance_static(se::State& s) // NOLINT(readability-identifier-naming)
{
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cc::experimental::Root* result = cc::experimental::Root::getInstance();
        ok &= nativevalue_to_se(result, s.rval(), nullptr /*ctx*/);
        SE_PRECONDITION2(ok, false, "Error processing arguments");
        SE_HOLD_RETURN_VALUE(result, s.thisObject(), s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_Root_getInstance_static)
static bool js_cc_experimental_Root_finalize(se::State& s) // NOLINT(readability-identifier-naming)
{
    return true;
}
SE_BIND_FINALIZE_FUNC(js_cc_experimental_Root_finalize)

bool js_register_chassis_Root(se::Object* obj) // NOLINT(readability-identifier-naming)
{
    auto* cls = se::Class::create("Root", obj, nullptr, nullptr);

#if CC_DEBUG
    cls->defineStaticProperty("isJSBClass", _SE(js_chassis_getter_return_true), nullptr);
#endif
    cls->defineFunction("createModel", _SE(js_chassis_Root_createModel));
    cls->defineFunction("createTransform", _SE(js_chassis_Root_createTransform));
    cls->defineFunction("destroy", _SE(js_chassis_Root_destroy));
    cls->defineFunction("initialize", _SE(js_chassis_Root_initialize));
    cls->defineFunction("render", _SE(js_chassis_Root_render));
    cls->defineStaticFunction("getInstance", _SE(js_chassis_Root_getInstance_static));
    cls->defineFinalizeFunction(_SE(js_cc_experimental_Root_finalize));
    cls->install();
    JSBClassType::registerClass<cc::experimental::Root>(cls);

    __jsb_cc_experimental_Root_proto = cls->getProto();
    __jsb_cc_experimental_Root_class = cls;


    se::ScriptEngine::getInstance()->clearException();
    return true;
}
se::Object* __jsb_cc_experimental_RootManager_proto = nullptr; // NOLINT
se::Class* __jsb_cc_experimental_RootManager_class = nullptr;  // NOLINT

static bool js_chassis_RootManager_create_static(se::State& s) // NOLINT(readability-identifier-naming)
{
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cc::experimental::Root* result = cc::experimental::RootManager::create();
        ok &= nativevalue_to_se(result, s.rval(), nullptr /*ctx*/);
        SE_PRECONDITION2(ok, false, "Error processing arguments");
        SE_HOLD_RETURN_VALUE(result, s.thisObject(), s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_RootManager_create_static)

static bool js_chassis_RootManager_destroy_static(se::State& s) // NOLINT(readability-identifier-naming)
{
    const auto& args = s.args();
    size_t argc = args.size();
    if (argc == 0) {
        cc::experimental::RootManager::destroy();
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_RootManager_destroy_static)

static bool js_chassis_RootManager_getInstance_static(se::State& s) // NOLINT(readability-identifier-naming)
{
    const auto& args = s.args();
    size_t argc = args.size();
    CC_UNUSED bool ok = true;
    if (argc == 0) {
        cc::experimental::Root* result = cc::experimental::RootManager::getInstance();
        ok &= nativevalue_to_se(result, s.rval(), nullptr /*ctx*/);
        SE_PRECONDITION2(ok, false, "Error processing arguments");
        SE_HOLD_RETURN_VALUE(result, s.thisObject(), s.rval());
        return true;
    }
    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 0);
    return false;
}
SE_BIND_FUNC(js_chassis_RootManager_getInstance_static)
static bool js_cc_experimental_RootManager_finalize(se::State& s) // NOLINT(readability-identifier-naming)
{
    return true;
}
SE_BIND_FINALIZE_FUNC(js_cc_experimental_RootManager_finalize)

bool js_register_chassis_RootManager(se::Object* obj) // NOLINT(readability-identifier-naming)
{
    auto* cls = se::Class::create("RootManager", obj, nullptr, nullptr);

#if CC_DEBUG
    cls->defineStaticProperty("isJSBClass", _SE(js_chassis_getter_return_true), nullptr);
#endif
    cls->defineStaticFunction("create", _SE(js_chassis_RootManager_create_static));
    cls->defineStaticFunction("destroy", _SE(js_chassis_RootManager_destroy_static));
    cls->defineStaticFunction("getInstance", _SE(js_chassis_RootManager_getInstance_static));
    cls->defineFinalizeFunction(_SE(js_cc_experimental_RootManager_finalize));
    cls->install();
    JSBClassType::registerClass<cc::experimental::RootManager>(cls);

    __jsb_cc_experimental_RootManager_proto = cls->getProto();
    __jsb_cc_experimental_RootManager_class = cls;


    se::ScriptEngine::getInstance()->clearException();
    return true;
}
bool register_all_chassis(se::Object* obj)    // NOLINT
{
    // Get the ns
    se::Value nsVal;
    if (!obj->getProperty("jsb", &nsVal, true))
    {
        se::HandleObject jsobj(se::Object::createPlainObject());
        nsVal.setObject(jsobj);
        obj->setProperty("jsb", nsVal);
    }
    se::Object* ns = nsVal.toObject();

    js_register_chassis_ModelView(ns);
    js_register_chassis_Root(ns);
    js_register_chassis_RootManager(ns);
    js_register_chassis_TransformView(ns);
    return true;
}

// clang-format on