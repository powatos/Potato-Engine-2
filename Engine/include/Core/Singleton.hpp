/** @file Singleton.hpp */
#pragma once

/**
 * @brief Base class for the singletons
 * * @tparam T The type of the singleton instance
 * @details This wrapper allows for easy creation and managing of singletons. Derive
 * from this class and use the target as the template argument to create a singleton. Ensure
 * the target class defines a private default constructor and the Singleton is allowed 
 * friend access. It is reccommended to define a destructor.
 * @note Inheriting from the singleton is limited. For robust singleton inheritence, consider
 * using [Meyer's pattern](https://en.wikipedia.org/wiki/Singleton_pattern), implemented manually.
 * @code
 * class MySingleton : public Singleton<MySingleton>
 * {
 *     friend class Singleton<MySingleton>;
 * private:
 *     MySingleton();
 *     ~MySingleton();
 * 
 *     // ...
 * };
 * @endcode
 */
template<typename T>
class Singleton
{
public:
    /**
     * @brief Gets singleton instance
     * @returns instance pointer
     */
    [[maybe_unused]] static T* Get() {
        return instance;
    }

    /**
     * @brief Makes instance of singleton
     * @return Created instance
     */
    static T* Once_MakeInstance() {
        static bool constructed = false;
        if (constructed) { return instance; }
        constructed = true;

        instance = new T();
        return Get();
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator = (const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator = (Singleton&&) = delete;


protected:
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    inline static T* instance;
};