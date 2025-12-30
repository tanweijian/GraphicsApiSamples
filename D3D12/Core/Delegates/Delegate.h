#pragma once

#include <unordered_set>

template<typename ReturnValueType, typename... Args>
class Delegate
{
public:
    using FunctionType = std::function<ReturnValueType(Args...)>;

    void Bind(ReturnValueType(*func)(Args...))
    {
        _func = func;
    }

    template<typename Class>
    void Bind(Class* obj, void (Class::* func)(Args...))
    {
        _func = [obj, func](Args... args) -> ReturnValueType { obj->*func(args...); };
    }

    template<typename Class>
    void Bind(Class* obj, ReturnValueType(Class::* func)(Args...) const)
    {
        _func = [obj, func](Args... args) -> ReturnValueType { obj->*func(args...); };
    }

    void Bind(FunctionType func)
    {
        _func = std::move(func);
    }

    void Unbind()
    {
        _func = nullptr;
    }

    void operator()(Args... args)
    {
        if (_func)
        {
            _func(std::forward<Args>(args)...);
        }
    }

private:
    FunctionType _func;
};

template<typename... Args>
class MulticastDelegate
{
public:
    using FunctionType = std::function<void(Args...)>;

    void Bind(void (*func)(Args...))
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _functions.insert(func);
    }

    template<typename Class>
    void Bind(Class* obj, void (Class::* func)(Args...))
    {
        _functions.insert([obj, func](Args... args) -> void { obj->*func(args...); });
    }

    template<typename Class>
    void Bind(Class* obj, void (Class::* func)(Args...) const)
    {
        _functions.insert([obj, func](Args... args) -> void { obj->*func(args...); });
    }

    void Bind(FunctionType func)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _functions.insert(func);
    }

    void UnbindAll()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _functions.clear();
    }

    void operator()(Args... args)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        for (auto& func : _functions)
        {
            if (func)
            {
                func(std::forward<Args>(args)...);
            }
        }
    }

private:
    std::mutex _mutex;
    std::unordered_set<FunctionType> _functions;
};

#define FUNC_DECLARE_DELEGATE(DelegateName, ReturnType, ...) \
    typedef Delegate<ReturnType(__VA_ARGS__)> DelegateName;

#define DECLARE_DELEGATE(DelegateName, ...) FUNC_DECLARE_DELEGATE(DelegateName, void, ...)

#define DECLARE_DELEGATE_RETVAL(ReturnValueType, DelegateName, ...) FUNC_DECLARE_DELEGATE(DelegateName, ReturnValueType, ...)

#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...) \
    typedef MulticastDelegate<__VA_ARGS__> DelegateName;