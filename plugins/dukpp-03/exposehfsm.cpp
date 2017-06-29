#include "dukpp-03/context.h"
#include "dukpp-03/jshfsmhandler.h"

#include "hfsm/hfsmstate.h"
#include "hfsm/hfsmshared.h"
#include "hfsm/hfsmmachine.h"
#include "hfsm/hfsmtransition.h"

#include <cassert>

#define PERFORM_AND_ASSERT(X)   {bool b = ctx->eval(X); assert(b); }


static sad::hfsm::AbstractHandler* _add_enter_handler(sad::hfsm::State* s, sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f)
{
    return s->handleEnterWith(new sad::dukpp03::JSHFSMHandler(ctx, f));
}

static sad::hfsm::AbstractHandler* _add_leave_handler(sad::hfsm::State* s, sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f)
{
    return s->handleLeaveWith(new sad::dukpp03::JSHFSMHandler(ctx, f));
}

static void _add_handler_for_transition(sad::hfsm::Transition* s, sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f)
{
    s->handleWith(new sad::dukpp03::JSHFSMHandler(ctx, f));
}

static void _add_state_1(sad::hfsm::Machine* m, const sad::String& s)
{
    m->addState(s);
}



void sad::dukpp03::exposeHFSM(sad::dukpp03::Context* ctx)
{
    PERFORM_AND_ASSERT("sad.hfsm = {};");

    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::hfsm::State>("SadHFSMState");
        c->addMethod("addChild", sad::dukpp03::bind_method::from(&sad::hfsm::State::addChild));
        c->addMethod("child", sad::dukpp03::bind_method::from(&sad::hfsm::State::child));
        c->addMethod("children", sad::dukpp03::bind_method::from(&sad::hfsm::State::children));
        c->addMethod("removeChild", sad::dukpp03::bind_method::from(&sad::hfsm::State::removeChild));
        c->addMethod("enter", sad::dukpp03::bind_method::from(&sad::hfsm::State::enter));
        c->addMethod("leave", sad::dukpp03::bind_method::from(&sad::hfsm::State::leave));
        c->addMethod("setParent", sad::dukpp03::bind_method::from(&sad::hfsm::State::setParent));
        c->addMethod("parent", sad::dukpp03::bind_method::from(&sad::hfsm::State::parent));
        c->addMethod("removeEnterHandler", sad::dukpp03::bind_method::from(&sad::hfsm::State::removeEnterHandler));
        c->addMethod("removeLeaveHandler", sad::dukpp03::bind_method::from(&sad::hfsm::State::removeLeaveHandler));
        c->addMethod("setShared", sad::dukpp03::bind_method::from(&sad::hfsm::State::setShared));
        c->addMethod("shared", sad::dukpp03::bind_method::from(&sad::hfsm::State::shared));

        c->addMethod("setVariable", sad::dukpp03::bind_method::from(&sad::hfsm::State::setVariable));
        c->addMethod("getVariable", sad::dukpp03::bind_method::from(&sad::hfsm::State::getVariable));

        c->addCloneValueObjectMethodFor<sad::hfsm::State>();
        c->setPrototypeFunction("SadHFSMState");

        ctx->addClassBinding("sad::hfsm::State", c);

        ctx->registerCallable("_SadHFSMStateAddEnterHandler", sad::dukpp03::make_function::from(_add_enter_handler));
        ctx->registerCallable("_SadHFSMStateAddLeaveHandler", sad::dukpp03::make_function::from(_add_leave_handler));

        PERFORM_AND_ASSERT("sad.hfsm.State = SadHFSMState;");

        PERFORM_AND_ASSERT("sad.hfsm.State.addEnterHandler = function(ctx, fn) { return _SadHFSMStateAddEnterHandler(this, ctx, fn); };");
        PERFORM_AND_ASSERT("sad.hfsm.State.addLeaveHandler = function(ctx, fn) { return _SadHFSMStateAddLeaveHandler(this, ctx, fn); };");
    }

    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::hfsm::Transition>("SadHFSMTransition");
        c->addMethod("invoke", sad::dukpp03::bind_method::from(&sad::hfsm::Transition::invoke));

        c->addCloneValueObjectMethodFor<sad::hfsm::Transition>();
        c->setPrototypeFunction("SadHFSMTransition");

        ctx->addClassBinding("sad::hfsm::Transition", c);

        ctx->registerCallable("_SadHFSMTransitionAddHandler", sad::dukpp03::make_function::from(_add_handler_for_transition));

        PERFORM_AND_ASSERT("sad.hfsm.Transition = SadHFSMTransition;");

        PERFORM_AND_ASSERT("sad.hfsm.Transition.addHandler = function(ctx, fn) { _SadHFSMTransitionAddHandler(this, ctx, fn); };");
    }

    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::hfsm::Shared>("SadHFSMShared");
        c->addMethod("setVariable", sad::dukpp03::bind_method::from(&sad::hfsm::Shared::setVariable));
        c->addMethod("getVariable", sad::dukpp03::bind_method::from(&sad::hfsm::Shared::getVariable));

        c->addCloneValueObjectMethodFor<sad::hfsm::Shared>();
        c->setPrototypeFunction("SadHFSMShared");

        ctx->addClassBinding("sad::hfsm::Shared", c);

        PERFORM_AND_ASSERT("sad.hfsm.Shared = SadHFSMShared;");
    }

    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::hfsm::Machine>("SadHFSMMachine");
        c->addMethod("enterState", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::enterState));
        c->addMethod("state", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::state));
        c->addMethod("_addState", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::addState));
        c->addMethod("removeState", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::removeState));
        c->addMethod("stateExists", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::stateExists));

        c->addMethod("addTransition", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::addTransition));
        c->addMethod("removeTransition", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::removeTransition));
        c->addMethod("transition", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::transition));
        c->addMethod("currentState", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::currentState));
        c->addMethod("previousState", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::previousState));
        c->addMethod("isInState", sad::dukpp03::bind_method::from(&sad::hfsm::Machine::isInState));

        c->addCloneValueObjectMethodFor<sad::hfsm::Machine>();
        c->setPrototypeFunction("SadHFSMMachine");

        ctx->addClassBinding("sad::hfsm::Machine", c);


        ctx->registerCallable("_SadHFSMMachineAdd1", sad::dukpp03::make_function::from(_add_state_1));

        PERFORM_AND_ASSERT("sad.hfsm.Machine = SadHFSMMachine;");

        PERFORM_AND_ASSERT("sad.hfsm.Machine.addState = function(name, state, force) { "
            "if (typeof state == \"undefined\") return _SadHFSMMachineAdd1(this, name);  "
            "if (typeof force == \"undefined\") return this._addState(name, state, false); "
            "return this._addState(name, state, force);"
            "};");

    }
}
